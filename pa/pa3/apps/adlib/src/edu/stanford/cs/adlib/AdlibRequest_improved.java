package edu.stanford.cs.adlib;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpUriRequest;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.math.BigInteger;

/* Class for computing random nonces for every adlib request */
final class NonceGenerator
{

  private SecureRandom random = new SecureRandom();

  public String newNonce()
  {
    return new BigInteger(130, random).toString(32);
  }

}

public class AdlibRequest {

	private static final String SECRET = "fakesecretkey";
	public final Set<String> keywords = new HashSet<String>();
	private static final NonceGenerator generator = new NonceGenerator();
	
	public void addKeyword(String keyword) {
		keywords.add(keyword);
	}
	
	/* return the hex string version of the SHA1 digest of input*/
	public static String SHA1(String input){
		MessageDigest md;
		
		try {
			md = MessageDigest.getInstance("SHA1");
		} catch (NoSuchAlgorithmException e) {
			return "";
		}
		
        md.update(input.getBytes()); 
     	byte[] hash = md.digest();
     	
     	StringBuffer hexString = new StringBuffer();

        for (int i = 0; i < hash.length; i++) {
        	if ((0xff & hash[i]) < 0x10) {
        		hexString.append("0" + Integer.toHexString((0xFF & hash[i])));
        	} else {
        		hexString.append(Integer.toHexString(0xFF & hash[i]));
        	}
        }
        
     	return hexString.toString();
	}

	protected HttpUriRequest toHttpGet(String baseUrl, String appId) {
		String nonce = generator.newNonce();
		String digest = SHA1(appId + SECRET + nonce);
		String url = baseUrl + "?keywords=" + this.keywordsToParamString() 
					+ "&app_id=" + appId + "&nonce=" + nonce + "&digest=" + digest;
		
		HttpGet request = new HttpGet(url);
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
	
}
