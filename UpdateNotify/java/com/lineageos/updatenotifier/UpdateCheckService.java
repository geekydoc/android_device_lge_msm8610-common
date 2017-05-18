package com.lineageos.updatenotifier;

import android.app.job.JobService;
import android.content.Intent;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.app.job.JobParameters;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.DateFormat;
import java.util.Date;
import org.xmlpull.v1.XmlPullParser;
import android.util.Xml;

public class UpdateCheckService extends JobService {

    private static final String TAG = "Update Check Service";
    public static final String BROADCAST_ACTION = "com.lineageos.updatenotifier.service_finished";
    private long builddate;
    private String buildname;
    private String changelogURL;

    @Override
    public boolean onStartJob(JobParameters params) {
        final JobParameters mparams = params;
        Log.i(TAG, "Service Started");
        new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    //Get the xml and parse the data
                    String URI = "https://geekydoc.github.io/update.xml";
                    URL url = new URL(URI);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.setRequestMethod("GET");
                    InputStream stream = conn.getInputStream();
                    XmlPullParser parser = Xml.newPullParser();
                    parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
                    parser.setInput(stream, null);
                    parser.nextTag();
                    int event;
                    while ((event = parser.next()) != XmlPullParser.END_DOCUMENT) {
                        if (event == XmlPullParser.START_TAG) {
                            String tag = parser.getName();
                            if ("update".equals(tag)) {
                                buildname = parser.getAttributeValue(null, "buildname");
                                builddate = Long.valueOf(parser.getAttributeValue(null,"builddate"));
                                changelogURL = parser.getAttributeValue(null, "changelogurl");
                            }
                        }
                    }

                    // Check data from above and change properties if necessary
                    PropertiesManager pm = new PropertiesManager(getApplicationContext());
                    if ((pm.getLong("last-notify")) < builddate) {
                        pm.setLong("last-notify", builddate);
                        pm.setString("last-package-available",buildname);
                        Util.sendNotification(getApplicationContext(), buildname, changelogURL);
                    }
                    pm.setString("last-check-date", DateFormat.getDateTimeInstance().format(new Date()));
                    // Inform the scheduler that the job is finished. We handle rescheduling ourselves
                    jobFinished(mparams,false);
                    if (pm.getBoolean("auto-check-enabled")) // Reschedule only if automatic check is enabled
                        Util.scheduleJob(getApplicationContext(), true);

                    send_finished_intent();
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
        Util.scheduleJob(getApplicationContext(), true);
        return false;
    }

    private void send_finished_intent(){
        Intent intent = new Intent(BROADCAST_ACTION);
        LocalBroadcastManager bm = LocalBroadcastManager.getInstance(getApplicationContext());
        bm.sendBroadcast(intent);
    }
}
