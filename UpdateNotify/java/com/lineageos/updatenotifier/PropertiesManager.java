package com.lineageos.updatenotifier;

import android.content.Context;

import java.io.IOException;
import java.io.InputStream;
import android.content.SharedPreferences;
import android.util.Log;
import java.util.Properties;


public class PropertiesManager {

    /*
    Available properties
    ---------------------
    auto-check-enabled [bool]: whether automatic update checks are enabled
    last-notify [long]: the UTC build time of the latest package the user was notified for
    last-check-date [string] : Last update check date and time
    last-package-available [string] : Name of last package notified for
    running-package [string] : Currently running package
     */

    private String TAG = "Update Notify Service - Properties Manager";
    private SharedPreferences prefs;
    private SharedPreferences.Editor editor ;


    public  PropertiesManager(Context context) {
        try {
            prefs = context.getSharedPreferences("Update-Notify-Prefs", Context.MODE_PRIVATE);
        }catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    public void update_buildprops() {
        try {
            Long builddate;
            Long last_notify = this.getLong("last-notify");
            String buildname;
            Process p = Runtime.getRuntime().exec("cat /system/build.prop");
            InputStream input = p.getInputStream();
            Properties props = new Properties();
            props.load(input);
            builddate = Long.parseLong(props.getProperty("ro.build.date.utc"));
            buildname = props.getProperty("ro.cm.version");
            if ( builddate > last_notify || last_notify == builddate ) {  //Clean ROM installation( last_notify returns default value 0) or Dirty flash
                this.setLong("last-notify", builddate);
            }
                this.setString("running-package",buildname);
                p.destroy();
            } catch (IOException e) {
                Log.e(TAG,e.toString());
        }
    }

    public Long getLong (String key) {
        return prefs.getLong(key, 0);
    }

    public boolean getBoolean(String key) {
        return prefs.getBoolean(key, false);
    }

    public String getString(String key) {return prefs.getString(key, "N/A");}

    public void setLong(String key, Long val)  {
        editor = prefs.edit();
        editor.putLong(key,val);
        editor.apply();
    }

    public void setString(String key, String string) {
        editor = prefs.edit();
        editor.putString(key,string);
        editor.apply();
    }

    public void setBoolean(String key, boolean val) {
        editor = prefs.edit();
        editor.putBoolean(key,val);
        editor.apply();
    }
}
