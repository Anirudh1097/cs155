package edu.stanford.cs.jack;

import cs.stanford.edu.jack.R;
import android.app.Activity;
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
        
        AdLibView adView = new AdLibView(this, "defc505d488e0a00ff92f7b0ac1c38c7");
        
        LinearLayout layout = (LinearLayout)findViewById(R.id.adView);
        layout.addView(adView);
        AdlibRequest adRequest = new AdlibRequest();
        adRequest.addKeyword("java");
		adView.loadAd(adRequest);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
}
