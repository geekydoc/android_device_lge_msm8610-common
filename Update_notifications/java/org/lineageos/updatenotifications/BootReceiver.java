package org.lineageos.updatenotifications;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BootReceiver extends BroadcastReceiver {



    @Override
    public void onReceive(final Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            Util.enter(context.getApplicationContext());
            Util.update_local_prefs();
            if (should_schedule())
                Util.scheduleJob( true);
        }
    }

    private boolean should_schedule(){
        return Util.sharedPrefs.getBoolean("auto_update_check_enabled", false);
    }

}
