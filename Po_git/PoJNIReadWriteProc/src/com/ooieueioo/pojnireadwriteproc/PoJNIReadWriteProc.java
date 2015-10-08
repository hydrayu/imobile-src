package com.ooieueioo.pojnireadwriteproc;


import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class PoJNIReadWriteProc extends Activity {

	private LinearLayout Po_LLay_read = null;
	private EditText Po_Read_ed = null;
	private TextView Po_Read_tv = null;
	private EditText Po_Write_ed = null;
	private EditText Po_Write_ed2 = null;
	private TextView Po_Write_tv = null;
	private String Read_Path = null;
	private String Write_Path = null;
	private String Write_value = null;
	
	static {
		System.loadLibrary("PoJNIT");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_po_jniread_write_proc);
		
		find_id();

	}
	
	public void Po_Write_btn (View view){
		// 1. get Write Path
		this.Write_Path = this.Po_Write_ed.getText().toString();
		
		// 2. get Write value
		this.Write_value = this.Po_Write_ed2.getText().toString();
		
		// 3. call WriteProc()
		String str = PoJNIT.WriteProc(this.Write_Path, this.Write_value);
		
		// 4. show message
		if(str.equals("oo")){
			String oo = " success";
			this.Po_Write_tv.setText("Write "+ this.Write_value +" --> " +this.Write_Path + oo);	
		}else{
			String xx = " error!";
			this.Po_Write_tv.setText("Write "+ this.Write_value +" --> " +this.Write_Path + xx);
		}
		
	}
	
	public void Po_Read_btn (View view){
		// 1. get Read_Path
		this.Read_Path = this.Po_Read_ed.getText().toString();
		
		// 2. call ReadProc()
		String str = PoJNIT.ReadProc(this.Read_Path);
		
		// 3. show JNI return value
		this.Po_Read_tv.setText("JNI return value ="+str);
	}
	
	private void find_id() {
		//Po_LLay_read = (LinearLayout) findViewById(R.id.Po_LLay_read);
		this.Po_Read_ed = (EditText) findViewById(R.id.Po_Read_ED1);
		this.Po_Read_tv = (TextView) findViewById(R.id.Po_Read_TV1);
		
		this.Po_Write_ed = (EditText) findViewById(R.id.Po_Write_ED1);
		this.Po_Write_ed2 = (EditText) findViewById(R.id.Po_Write_ED2);
		this.Po_Write_tv = (TextView) findViewById(R.id.Po_Write_TV1);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.po_jniread_write_proc, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
