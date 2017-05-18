package com.lineageos.updatenotifier;


import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.job.JobInfo;
import android.app.job.JobScheduler;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import java.util.concurrent.TimeUnit;

public class Util {

    private final static int JOBID = 555;

    protected static void scheduleJob(Context context, boolean withlatency){
        ComponentName serviceComponent=new ComponentName(context,UpdateCheckService.class);
        JobInfo.Builder builder=new JobInfo.Builder(JOBID,serviceComponent);
        if (withlatency) {
            builder.setMinimumLatency(TimeUnit.DAYS.toMillis(5)); //schedule it after 5 days
            builder.setPersisted(true); // Reschedule the job on reboot
        }
        builder.setRequiredNetworkType(JobInfo.NETWORK_TYPE_ANY); // require any network type
        JobScheduler jobScheduler=context.getSystemService(JobScheduler.class);
        jobScheduler.schedule(builder.build());
    }

    protected  static void cancel_job(Context context){
        JobScheduler jobscheduler = context.getSystemService(JobScheduler.class);
        jobscheduler.cancel(JOBID);
    }

    protected static void sendNotification(Context context, String buildname, String changelogURL){

        Intent click_event = new Intent(Intent.ACTION_VIEW, Uri.parse(changelogURL));
        PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, click_event, 0);
        Notification n  = new Notification.Builder(context)
                .setContentTitle("Package: "+ buildname)
                .setSubText("New Update Available")
                .setContentText("Touch for more information")
                .setSmallIcon(R.drawable.logo)
                .setContentIntent(pendingIntent)
                .setAutoCancel(true).build();
        NotificationManager mNotificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.notify(101, n);
    }

}
