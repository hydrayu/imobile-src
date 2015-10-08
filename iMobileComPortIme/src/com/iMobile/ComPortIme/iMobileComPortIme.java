/*
 * Copyright (C) 2008-2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * IME lifecycle
 * InputMethodService start
 * onCreate()
 * onCreateInputView()
 * onCreateCandidateViews()
 * onStartInputView()
 * Text Input gets the current Input method subtype
 * InputMethodManager#getCurrentINputMethodSubtype()
 * Text Input has started
 * onCurrentInputMethodSubtypeChanged()
 * Detect the current Input method subtype has been changed
 *    (repeat "Text Input gets the current Input method subtype", and then...)
 * onFinishInput()
 *    (Move to an additional field, and then...)
 * onDestroy()
 * InputMethodService stops
 */
package com.iMobile.ComPortIme;

/*
 * IME 的兩大組成:
 * 1. android.inputmethodservice
 * 		a. 當然最重要的是，它是個 Service : InputMethodService
 * 		b. 需要有 Keyboard 物件，據說是個 XML, 以及
 * 		c. Keyboard 外觀 : KeyboardView
 */
import android.inputmethodservice.InputMethodService;
import android.inputmethodservice.KeyboardView;
import android.inputmethodservice.Keyboard;

/*
 * 2. android.view.inputmethod
 * 		a. 用來處理鍵盤 UI 與輸入框，所以最重要的是 InputConnection 來當中間介面
 * 			但是我們不需要特別處理輸入，所以不必 import
 * 			import android.inputmethod.InputConnection;
 * 		b. 目標是 EditText, 所以要有一個界面 : EditorInfo
 */
import android.view.View;
import android.view.inputmethod.EditorInfo;

/*
 * imports for Serial Port 
 */
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.InvalidParameterException;

import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android_serialport_api.SerialPort;

/*
 * other imports
 */
import android.widget.Toast;
import com.iMobile.ComPortIme.ImePreferences;

/**
 * Example of writing an input method for a soft keyboard.  This code is
 * focused on simplicity over completeness, so it should in no way be considered
 * to be a complete soft keyboard implementation.  Its purpose is to provide
 * a basic example for how you would get started writing an input method, to
 * be fleshed out as appropriate.
 */
public class iMobileComPortIme extends InputMethodService
        implements KeyboardView.OnKeyboardActionListener {
    private KeyboardView mInputView;
    private Keyboard mComPortImeKeyboard;
    private SerialPort mSerialPort = null;
    private ReadThread mReadThread;
    private InputStream mComIn;

    /**
     * Step 1, onCreate()
     * 進設定畫面就執行了...想必是任何啟動它的都會執行到
     */
    @Override public void onCreate() {
        super.onCreate();
    }
    
    /**
     * Step 1.1, onInitializeInterface()
     * 時機上跟 onCreate() 應該一樣的吧
     * 第一次叫輸入法起來，例如輸入框，或是設定應該都會執行到
     */
    @Override public void onInitializeInterface() {
    	mComPortImeKeyboard = new Keyboard(this, R.xml.comime);
		SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
		String COM = sp.getString("DEVICE",  "");
		int baudrate = Integer.decode(sp.getString("BAUDRATE",  "-1"));
		if ((COM.length() == 0) || (baudrate == -1)) {
			Intent it = new Intent(this, ImePreferences.class);
			it.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(it);
		}
    }
    
    /**
     * Step 2. onCreateInputView()
     * 第一次叫出輸入法鍵盤時會呼叫到
     */
    @Override public View onCreateInputView() {
        mInputView = (KeyboardView) getLayoutInflater().inflate(
                R.layout.input, null);
        mInputView.setOnKeyboardActionListener(this);
        mInputView.setKeyboard(mComPortImeKeyboard);
        return mInputView;
    }

    /*
     * Step 3, onCreateCandidateViews()
     * Skip it....we have no Candidate View...
     */
    
    /*
     * Step 4, onStartInputView(
     * When an input field receives focus and your IME starts, 
     * the system calls onStartInputView()
     */
    @Override public void onStartInputView(EditorInfo attribute, boolean restarting) {
        super.onStartInputView(attribute, restarting);
        mInputView.setKeyboard(mComPortImeKeyboard);
		try {
			mSerialPort = getSerialPort();
		}
		catch (InvalidParameterException e) { e.printStackTrace(); }
		catch (SecurityException e) 		{ e.printStackTrace(); }
		catch (IOException e) 				{ e.printStackTrace(); }
        if (mReadThread == null) {
        	mReadThread = new ReadThread();
            mReadThread.start();
        }
		if (mSerialPort.getDevPath().indexOf("ttyMT0") >= 0) {
	    	try {
				enableBarcodeReader();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
        mInputView.closing();
    }
    
    protected void onDataReceived(final byte[] buffer, final int size) {
    	getCurrentInputConnection().commitText((new String(buffer))+"\n", 0);
		String COM = PreferenceManager.getDefaultSharedPreferences(this).getString("DEVICE",  "");
		if (COM.indexOf("ttyMT0") >= 0) {
	    	try {
				enableBarcodeReader();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
    }

    /*
     * Step 5, getCurrentInputMethodSubtype()
     * Skip it, we don'e support subtype
     */
    
    /**
     * Step 6, onStartInput()
     * 可以在此偵測輸入框的型態，譬如
     * Phone, Datetime, Number, Email, Url, TextEdit..
     * 後面還有半步驟，就是真正接受輸入，放最後面比較簡潔
     * 包含切到桌面、輸入法、設定什麼的，都會呼叫到!!!!
     */
    @Override public void onStartInput(EditorInfo attribute, boolean restarting) {
        super.onStartInput(attribute, restarting);
		if (true) { // mSerialPort.getDevPath().indexOf("ttyMT0") >= 0) {
	        try {
				disableBarcodeReader();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
    }

    /*
     * Step 7, onCurrentInputMethodSubtypeChanged()
     * Skip it, we don't support subtype
     */
    
    /**
     * Step 8, onFinishInput()
     * 輸入法關閉時會呼叫到，例如進入待機
     */
    @Override public void onFinishInput() {
        super.onFinishInput();
        if (mInputView != null) {
            mInputView.closing();
        }
    }
    /*
     * Step 9, onDestroy()
     * We have no onDestroy()? I think the onFinishInput() is enough
     */
    @Override public void onDestroy() {
        mSerialPort.close();
        if (mReadThread != null) mReadThread.interrupt();
        super.onDestroy();
    }
    
    /*
     * Step 6.1, 真正輸入在此
     *  Implementation of KeyboardViewListener
     *  事實上，有 COM Port 輸入也不必實作在此，而是用...
     *    getCurrentInputConnection().commitText()
     */
    public void onKey(int primaryCode, int[] keyCodes) {
		Toast.makeText(this, "Enter \"Setup\" -> \"Language & Input\"\n to configure the COM Port" ,
				Toast.LENGTH_LONG).show();
    }

    /*
     * Step 6.2, other implements
     * 底下只是必要的實作，作為輸入法，理論接受實體鍵盤，雖然我們不需要
     */
    // onText() 好像是有 Candidate 可以輸入整串的，我們用不到
    public void onText(CharSequence text) {}
    public void swipeRight() {}
    public void swipeLeft() {}
    public void swipeDown() {}
    public void swipeUp() {}
    public void onPress(int primaryCode) {}
    public void onRelease(int primaryCode) {}

    public SerialPort getSerialPort() throws SecurityException, IOException, InvalidParameterException {
    	if (mSerialPort != null) mSerialPort.close();
		SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(this);
		String COM = sp.getString("DEVICE",  "");
		int baudrate = Integer.decode(sp.getString("BAUDRATE",  "-1"));

		if ((COM.length() == 0) || (baudrate == -1)) {
			throw new InvalidParameterException();
		}
		else {
    		mSerialPort = new SerialPort(new File(COM), baudrate, 0);
    		mComIn = mSerialPort.getInputStream();
		}
    	return mSerialPort;
    }
    
    public void closeSerialPort() {
    	if (mSerialPort != null) {
    		mSerialPort.close();
    		mSerialPort = null;
    	}
    }
    
    public void enableBarcodeReader() throws FileNotFoundException, IOException, InterruptedException
    {
    	File gpio = new File("/proc/gpio");
    	FileOutputStream gpioOS = new FileOutputStream(gpio);
    	gpioOS.write("75 1 0 1".getBytes());
    	Thread.sleep(500);
    	gpioOS.write("75 0 0 1".getBytes());
    	gpioOS.close();
    }
    
    public void disableBarcodeReader() throws FileNotFoundException, IOException, InterruptedException
    {
    	File gpio = new File("/proc/gpio");
    	FileOutputStream gpioOS = new FileOutputStream(gpio);
    	gpioOS.write("75 1 0 1".getBytes());
    	gpioOS.close();
    }
    
    private class ReadThread extends Thread {
    	@Override
    	public void run() {
    		super.run();
    		while (!isInterrupted()) {
    			int size;
    			try {
    				if (mComIn == null) return;
    				byte[] buffer = new byte[64];
    				size = mComIn.read(buffer);
    				if (size > 0) {
    					onDataReceived(buffer, size);
    				}
    			} catch (IOException e) {
    				e.printStackTrace();
    				return;
    			}
    		}
    	}
    }
}