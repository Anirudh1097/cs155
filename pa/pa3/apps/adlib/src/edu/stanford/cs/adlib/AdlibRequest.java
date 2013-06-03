package edu.stanford.cs.adlib;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;

public class AdlibRequest {

	private static final String SECRET = "fakesecretkey";
	public final Set<String> keywords = new HashSet<String>();
	
	public void addKeyword(String keyword) {
		keywords.add(keyword);
	}

	protected HttpUriRequest toHttpGet(String baseUrl, String appId) {
		
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
