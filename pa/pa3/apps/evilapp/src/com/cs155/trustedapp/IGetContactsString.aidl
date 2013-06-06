/* IGetContactsString.aidl 
 * 
 * Interface to the ReadContactService service for the TrustedApp
 * application. Basically allows other apps to do IPC with the
 * TrustedApp's ReadContactService service. This interface allows
 * us to cast IBinder objects given to ServiceConnection's
 * onServiceConnected method to IGetContactsString's whenever an 
 * app binds to the TrustedApp's ReadContactService service. We 
 * can then use IGetContactsString's GetContacts method to get 
 * the contacts.
 */

package com.cs155.trustedapp;

interface IGetContactsString {

    String GetContacts(String paramString);

}