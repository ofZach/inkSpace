package cc.openframeworks.accelDraw;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.nio.channels.FileChannel;
import java.util.Calendar;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.Date;
import java.text.SimpleDateFormat;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import cc.openframeworks.OFAndroid;
import android.util.Log;

public class OFActivity extends cc.openframeworks.OFActivity{

	
	private static Context context;
	 
	public static Boolean copyFile(File msg, File destFile)
		      throws IOException {
		    if (!destFile.exists()) {
		      destFile.createNewFile();

		      FileChannel source = null;
		      FileChannel destination = null;
		      try {
		        source = new FileInputStream(msg).getChannel();
		        destination = new FileOutputStream(destFile).getChannel();
		        destination.transferFrom(source, 0, source.size());
		      } finally {
		        if (source != null)
		          source.close();
		        if (destination != null)
		          destination.close();
		      }
		      return true;
		    }
		    return false;
		  }
		
	
	public static void sayHi(String msg){  
		
//		Intent intent = new Intent();
//		intent.setAction(Intent.ACTION_VIEW);
//		intent.setDataAndType(Uri.parse("file://" + msg), "image/*");
//		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//		context.startActivity(intent);
		
		
		
		 File path = Environment.getExternalStoragePublicDirectory(
			     Environment.DIRECTORY_PICTURES
			  );
		 
		 Date today = Calendar.getInstance().getTime();
		 SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd-hh-mm-ss");
		 String dateString = formatter.format(today);
		    
		 
		 
		  File imageFileName = new File(path, dateString + ".gif"); //imageFileFolder

		  
		  File temp = new File(msg);
		 
		  try {
			copyFile(temp, imageFileName);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		  
		  scanPhoto(imageFileName.toString());
	
		  //Log.d("???", imageFileName.toString());
		
		  
		  Intent intent = new Intent();
		  intent.setAction(Intent.ACTION_VIEW);
		  intent.setDataAndType(Uri.parse("file://" + imageFileName.toString()), "image/*");
		  intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		  context.startActivity(intent);
		  
		//context.sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://"+ Environment.getExternalStorageDirectory())));
		
		//Intent i = new Intent(
			//	Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
				 
		//context.startActivityForResult(i, RESULT_LOAD_IMAGE);
		
	   
//		Log.d("d","wha???");
//		Log.d("d",msg);
//		Log.d("d","ha!!");
	}
	
	private static void scanPhoto(String imageFileName)
	{
	  Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
	  File f = new File(imageFileName);
	  Uri contentUri = Uri.fromFile(f);
	  mediaScanIntent.setData(contentUri);
	  //this.cordova.getContext().sendBroadcast(mediaScanIntent); //this is deprecated
	  context.sendBroadcast(mediaScanIntent); 
	}
	
	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
        super.onCreate(savedInstanceState);
        String packageName = getPackageName();

        ofApp = new OFAndroid(packageName,this);
        
        context = getApplicationContext();
		
    }
	
	@Override
	public void onDetachedFromWindow() {
	}
	
    @Override
    protected void onPause() {
        super.onPause();
        ofApp.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ofApp.resume();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
	if (OFAndroid.keyDown(keyCode, event)) {
	    return true;
	} else {
	    return super.onKeyDown(keyCode, event);
	}
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
	if (OFAndroid.keyUp(keyCode, event)) {
	    return true;
	} else {
	    return super.onKeyUp(keyCode, event);
	}
    }


	OFAndroid ofApp;
    
	
	
    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	// Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	// This passes the menu option string to OF
    	// you can add additional behavior from java modifying this method
    	// but keep the call to OFAndroid so OF is notified of menu events
    	if(OFAndroid.menuItemSelected(item.getItemId())){
    		
    		return true;
    	}
    	return super.onOptionsItemSelected(item);
    }
    

    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
    	// This method is called every time the menu is opened
    	//  you can add or remove menu options from here
    	return  super.onPrepareOptionsMenu(menu);
    }
	
}



