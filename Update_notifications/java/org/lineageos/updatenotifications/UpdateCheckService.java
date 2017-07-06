package org.lineageos.updatenotifications;

import android.app.job.JobService;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.app.job.JobParameters;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.DateFormat;
import java.util.Date;
import java.util.Properties;

public class UpdateCheckService extends JobService {

    private static final String TAG = "Update Check Service";


    @Override
    public boolean onStartJob(JobParameters params) {
        final JobParameters mparams = params;
        Log.i(TAG, "Service Started");
        new Thread(new Runnable() {
            @Override
            public void run() {
                Util.enter(getApplicationContext());
                try {
                    //Get the properties file and parse the data
                    String URI = "https://geekydoc.github.io/latest.prop";
                    URL url = new URL(URI);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.setRequestMethod("GET");
                    InputStream stream = conn.getInputStream();
                    Properties Props = new Properties();
                    Props.load(stream);
                    // Server Properties
                    Long server_builddate = Long.parseLong(Props.getProperty("builddate.utc"));
                    String server_buildname = Props.getProperty("buildname");
                    stream.close();

                    //Local Shared Preferences

                    Long builddate_running = Util.sharedPrefs.getLong("builddate.utc.running",0);
                    Long builddate_notified = Util.sharedPrefs.getLong("builddate.utc.notified", 0);

                    SharedPreferences.Editor editor = Util.sharedPrefs.edit();
                    // Compare local and server data and change local properties as needed
                    if (builddate_running < server_builddate)
                        if (builddate_notified < server_builddate)
                            Util.sendNotification(getApplicationContext(), server_buildname);
                            editor.putLong("builddate.utc.notified",server_builddate);
                            editor.putString("buildname.latest",server_buildname);

                    editor.putString("last-check-date", DateFormat.getDateTimeInstance().format(new Date()));
                    editor.commit();
                    send_finished_intent();
                    jobFinished(mparams,false);
                    if (Util.sharedPrefs.getBoolean("auto_update_check_enabled",false))
                        Util.scheduleJob( false);
                } catch (Exception e) {
                    Log.e(TAG, e.toString());
                }
            }
        }).start();

        return true;
    }

    @Override
    public boolean onStopJob(JobParameters params){
        //Job was stopped before being able to finish. Just reschedule for now
        Util.scheduleJob( false);
        return false;
    }

    private void send_finished_intent(){
        Intent intent = new Intent(Util.SERVICE_FINISHED_BROADCAST_ACTION);
        LocalBroadcastManager bm = LocalBroadcastManager.getInstance(getApplicationContext());
        bm.sendBroadcast(intent);
    }
}