package edu.stanford.cs.adlib;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;

import android.content.Context;
import android.net.http.AndroidHttpClient;
import android.os.AsyncTask;
import android.util.AttributeSet;
import android.util.Log;
import android.webkit.WebView;
import android.widget.RelativeLayout;

public class AdLibView extends RelativeLayout {

	private static final String BASE_URL = "http://adlib.mappend.net/token";
	private static final String USER_AGENT = "adlib-android";
	private final AndroidHttpClient httpClient = AndroidHttpClient
			.newInstance(USER_AGENT);
	private WebView webView;
	private String appId;

	public AdLibView(Context context, String appId) {
		super(context);
		this.appId = appId;
		this.setLayoutParams(new LayoutParams(800, 100));
		webView = new WebView(context);
		this.addView(webView);
	}

	public void loadAd(AdlibRequest adRequest) {
		new GetRequest(adRequest.toHttpGet(BASE_URL, appId)) {
			@Override
			protected void onPostExecute(HttpResponse result) {
				if (result != null) {
					try {
						Reader reader = new InputStreamReader(result.getEntity().getContent());
						char buf[] = new char[4096];
						StringBuilder builder = new StringBuilder();
						while (reader.read(buf) > 0) {
							builder.append(buf);
						}
						webView.loadDataWithBaseURL(BASE_URL, builder.toString(), "text/html", "utf-8", "#");
					} catch (Exception e) {
						Log.d("AdlibView", "", e);
					}
				} else {
					webView.loadData("Failed...", "text/html", "utf-8");
				}
			}
		}.execute();
	}

	public AdLibView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	private class GetRequest extends AsyncTask<Void, Void, HttpResponse> {

		private HttpUriRequest adRequest;

		public GetRequest(HttpUriRequest req) {
			this.adRequest = req;
		}
		
		@Override
		protected HttpResponse doInBackground(Void... requests) {
			try {
				HttpResponse response = httpClient.execute(adRequest);
				return response;
			} catch (IOException e) {
				return null;
			}
		}
		
	}

}
