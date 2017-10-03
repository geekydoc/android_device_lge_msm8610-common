package org.lineageos.updatenotifications;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.support.v4.content.LocalBroadcastManager;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

public class SettingsActivity extends Activity {

        private TextView txt_running_package;
        private TextView txt_last_available;
        private TextView txt_last_check_date;
        private LocalBroadcastManager bm;

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(null);
            Util.enter(getApplicationContext());
            setContentView(R.layout.activity_settings);
            txt_running_package = (TextView)findViewById(R.id.txt_running_package);
            txt_last_available = (TextView)findViewById(R.id.txt_last_available);
            txt_last_check_date = (TextView)findViewById(R.id.txt_last_check_date);


            //Check the shared preferences, if builddate.utc.current=0, regenerate

            if (Util.sharedPrefs.getLong("builddate.utc.running",0) == 0)
                Util.update_local_prefs();

            Switch switch_enable;
            switch_enable = (Switch)findViewById(R.id.switch_enable);
            switch_enable.setChecked(Util.sharedPrefs.getBoolean("auto_update_check_enabled",false));
            switch_enable.setOnCheckedChangeListener(listener);

            updateUI();
        }

        private boolean isConnected() {
            ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo nInfo = connectivityManager.getActiveNetworkInfo();
            return nInfo!=null && nInfo.isConnected();
        }

        private void updateUI() {
            Context mcontext = getApplicationContext();
            txt_running_package.setText(Util.sharedPrefs.getString("buildname.running",mcontext.getResources().getString(R.string.nodata)));
            txt_last_available.setText((Util.sharedPrefs.getString("buildname.latest",mcontext.getResources().getString(R.string.nodata_recheck))));
            txt_last_check_date.setText((Util.sharedPrefs.getString("last-check-date",mcontext.getResources().getString(R.string.nodata_recheck))));
        }


        private CompoundButton.OnCheckedChangeListener listener = new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                SharedPreferences.Editor editor = Util.sharedPrefs.edit();
                if (isChecked) {
                    editor.putBoolean("auto_update_check_enabled", true);
                    editor.commit();
                    Util.scheduleJob( true);
                    updateUI_on_service_completion();

                } else {
                    editor.putBoolean("auto_update_check_enabled", false);
                    editor.commit();
                    Util.cancel_job();
                }
            }
        };

        private void updateUI_on_service_completion(){
            txt_last_check_date.setText(R.string.check_in_progress);
            IntentFilter filter = new IntentFilter();
            filter.addAction(Util.SERVICE_FINISHED_BROADCAST_ACTION);
            bm = LocalBroadcastManager.getInstance(this);
            final BroadcastReceiver service_finished_receiver = new BroadcastReceiver() {

                @Override
                public void onReceive(Context context, Intent intent) {
                    updateUI();
                    bm.unregisterReceiver(this);
                }
            };


            bm.registerReceiver(service_finished_receiver, filter);
        }

        public void button_click(View view) {

            // Check if internet is connected
            if (isConnected()) {
                Util.scheduleJob( true);
                updateUI_on_service_completion();
            }else{
                Toast.makeText(getApplicationContext(),R.string.connect_retry,Toast.LENGTH_LONG).show();
            }
        }
}
