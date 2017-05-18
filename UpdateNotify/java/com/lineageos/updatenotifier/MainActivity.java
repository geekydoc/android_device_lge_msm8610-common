package com.lineageos.updatenotifier;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

    private Context context;
    private PropertiesManager pm;

    private TextView last_package_available_text;
    private TextView last_check_date_text;
    private LocalBroadcastManager bm;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(null);
        setContentView(R.layout.activity_main);
        context = getApplicationContext();
        pm = new PropertiesManager(context);
        pm.update_buildprops();
        TextView running_package_textview;
        running_package_textview = (TextView) findViewById(R.id.package_curr_textview);
        last_package_available_text = (TextView) findViewById(R.id.package_latest_textview);
        last_check_date_text = (TextView) findViewById(R.id.last_check_date_textview);
        Switch check_switch;
        check_switch = (Switch) findViewById(R.id.check_switch);
        running_package_textview.setText("Running package: " + pm.getString("running-package"));
        check_switch.setChecked(pm.getBoolean("auto-check-enabled"));
        if (check_switch.isChecked()) {
            this.updateUI();
        }
        check_switch.setOnCheckedChangeListener(listener);

    }

    private boolean isConnected() {
        ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo nInfo = connectivityManager.getActiveNetworkInfo();
        return nInfo!=null && nInfo.isConnected();
    }


    private void disableUI() {
        last_package_available_text.setText("Latest package Available: ");
    }


    private void updateUI() {
        last_package_available_text.setText("Latest package available: " + pm.getString("last-package-available"));
        last_check_date_text.setText(pm.getString("last-check-date"));
    }



    private CompoundButton.OnCheckedChangeListener listener = new CompoundButton.OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            if (isChecked) {
                pm.setBoolean("auto-check-enabled", true);
                Util.scheduleJob(context, false);
                updateUI();

            } else {
                pm.setBoolean("auto-check-enabled", false);
                Util.cancel_job(context);
                disableUI();
            }
        }
    };

    public void buttonclick(View view) {
        // Check if internet is connected
        if (isConnected()) {
            Util.scheduleJob(context, false);
            IntentFilter filter = new IntentFilter();
            filter.addAction(UpdateCheckService.BROADCAST_ACTION);
            bm = LocalBroadcastManager.getInstance(this);
            final BroadcastReceiver service_finished_receiver = new BroadcastReceiver() {

                @Override
                public void onReceive(Context context, Intent intent) {
                    updateUI();
                    bm.unregisterReceiver(this);
                }
            };


            bm.registerReceiver(service_finished_receiver, filter);
        }else{
            Toast.makeText(context,"Connect to the Internet and Retry",Toast.LENGTH_LONG).show();
        }
    }

}