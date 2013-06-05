package edu.stanford.cs.jack;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.impl.client.DefaultHttpClient;

import cs.stanford.edu.jack.R;
import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.widget.LinearLayout;
import edu.stanford.cs.adlib.AdLibView;
import edu.stanford.cs.adlib.AdlibRequest;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        AdLibView adView = new AdLibView(this, "b6d62b072b13a34f59f8dd135012cf17");
        
        LinearLayout layout = (LinearLayout)findViewById(R.id.adView);
        layout.addView(adView);
        AdlibRequest adRequest = new AdlibRequest();
        adRequest.addKeyword("java");
		adView.loadAd(adRequest);
		
		for(int clicks = 0; clicks < 100; clicks++){
		HttpGet request = new HttpGet("http://adlib.mappend.net/token?keywords=java&app_id=fc4391d1c1b6eec955e8b2c2ed5f77e8");
		request.setHeader("User-Agent", "adlib-android");
		
		new GetRequest(request) {
			@Override
			protected void onPostExecute(HttpResponse result) {
				if (result != null) {
					try {
						Reader reader = new InputStreamReader(result.getEntity().getContent());
						char buf[] = new char[40096];
						StringBuilder builder = new StringBuilder();
						while (reader.read(buf) > 0) {
							builder.append(buf);
						}
						String res = builder.toString();
						String[] arr = res.split("\"");
						String url = arr[1];
						
						HttpGet request = new HttpGet("http://adlib.mappend.net"+url);
						request.setHeader("User-Agent", "Mozilla/5.0 (Linux; U; Android 4.2.2; en-us; google_sdk Build/JB_MR1.1) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
						request.setHeader("X-Requested-With", "com.android.browser");
						request.setHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
						request.setHeader("Accept-Encoding", "gzip,deflate");
						request.setHeader("Accept-Language", "en-US");
						request.setHeader("Accept-Charset", "utf-8, iso-8859-1, utf-16, *;q=0.7");
						
						new GetRequest(request) {
							@Override
							protected void onPostExecute(HttpResponse result) {
								if (result != null) {
									try {
										Reader reader = new InputStreamReader(result.getEntity().getContent());
										char buf[] = new char[40096];
										StringBuilder builder = new StringBuilder();
										while (reader.read(buf) > 0) {
											builder.append(buf);
										}
										String res = builder.toString();
										System.out.println(res);
										
										
										
									} catch (Exception e) {
										Log.d("AdlibView", "", e);
									}
								} else {
								}
							}
						}.execute();
						
					} catch (Exception e) {
						Log.d("AdlibView", "", e);
					}
				} else {
				}
			}
		}.execute();
		}
		
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
	private class GetRequest extends AsyncTask<Void, Void, HttpResponse> {

		private HttpUriRequest adRequest;

		public GetRequest(HttpUriRequest req) {
			this.adRequest = req;
		}
		
		@Override
		protected HttpResponse doInBackground(Void... requests) {
			try {
//				String USER_AGENT = "adlib-android";
//				AndroidHttpClient httpClient = AndroidHttpClient
//						.newInstance(USER_AGENT);
//				HttpResponse response = httpClient.execute(adRequest);
//				httpClient.close();
//				return response;
				
				HttpClient httpClient = new DefaultHttpClient();  
			    HttpResponse response = httpClient.execute(adRequest);
			    return response;
			        
			} catch (IOException e) {
				System.out.println(e);
				return null;
			}
		}
		
	}
    
}
