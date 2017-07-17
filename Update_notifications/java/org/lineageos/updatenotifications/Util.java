package org.lineageos.updatenotifications;


import android.app.Notification;
import android.app.NotificationManager;
import android.app.job.JobInfo;
import android.app.job.JobScheduler;
import android.content.ComponentName;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import static android.content.ContentValues.TAG;

final class Util {

    final static int JOBID = 555;
    final static String SERVICE_FINISHED_BROADCAST_ACTION = "org.lineageos.updatenotifications.service_finished";

    static SharedPreferences sharedPrefs;
    static Context mcontext;

    private Util() {
    }

    static void enter(Context context){
        mcontext = context;
        sharedPrefs = PreferenceManager.getDefaultSharedPreferences(mcontext);
    }

    static void scheduleJob(boolean now) {
        ComponentName serviceComponent = new ComponentName(mcontext, UpdateCheckService.class);
        JobInfo.Builder builder = new JobInfo.Builder(JOBID, serviceComponent);
        builder.setRequiredNetworkType(JobInfo.NETWORK_TYPE_ANY);
        if (!now)
			builder.setMinimumLatency(432000000);
        JobScheduler jobScheduler = mcontext.getSystemService(JobScheduler.class);
        jobScheduler.schedule(builder.build());
    }


    static void cancel_job() {
        JobScheduler jobscheduler = mcontext.getSystemService(JobScheduler.class);
        jobscheduler.cancel(JOBID);
    }

    static void update_local_prefs(){
        try
        {
        Process p = Runtime.getRuntime().exec("cat /system/build.prop");
        InputStream input = p.getInputStream();
        Properties Props = new Properties();
        Props.load(input);
        Long builddate = Long.parseLong(Props.getProperty("ro.build.date.utc"));
        String buildname = Props.getProperty("ro.cm.version", "Could not read data");
        p.destroy();
        input.close();
        SharedPreferences.Editor editor = sharedPrefs.edit();
        editor.putLong("builddate.utc.running", builddate);
        editor.putString("buildname.running", buildname);
        editor.commit();
        } catch(IOException e) {
        Log.e(TAG, e.toString());
        }
    }

    static void sendNotification(Context context, String buildname) {

        Notification n = new Notification.Builder(context)
                .setContentTitle(context.getResources().getString(R.string.notif_title)+ buildname)
                .setSubText(context.getResources().getString(R.string.notif_subtitle))
                .setContentText(context.getResources().getString(R.string.notif_content))
                .setSmallIcon(R.drawable.logo)
                .setAutoCancel(false).build();
        NotificationManager mNotificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.notify(101, n);
    }
}
