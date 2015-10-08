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
 * IME ����j�զ�:
 * 1. android.inputmethodservice
 * 		a. ��M�̭��n���O�A���O�� Service : InputMethodService
 * 		b. �ݭn�� Keyboard ����A�ڻ��O�� XML, �H��
 * 		c. Keyboard �~�[ : KeyboardView
 */
import android.inputmethodservice.InputMethodService;
import android.inputmethodservice.KeyboardView;
import android.inputmethodservice.Keyboard;

/*
 * 2. android.view.inputmethod
 * 		a. �ΨӳB�z��L UI �P��J�ءA�ҥH�̭��n���O InputConnection �ӷ�������
 * 			���O�ڭ̤��ݭn�S�O�B�z��J�A�ҥH���� import
 * 			import android.inputmethod.InputConnection;
 * 		b. �ؼЬO EditText, �ҥH�n���@�Ӭɭ� : EditorInfo
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
     * �i�]�w�e���N����F...�Q���O����Ұʥ������|�����
     */
    @Override public void onCreate() {
        super.onCreate();
    }
    
    /**
     * Step 1.1, onInitializeInterface()
     * �ɾ��W�� onCreate() ���Ӥ@�˪��a
     * �Ĥ@���s��J�k�_�ӡA�Ҧp��J�ءA�άO�]�w���ӳ��|�����
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
     * �Ĥ@���s�X��J�k��L�ɷ|�I�s��
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
     * �i�H�b��������J�ت����A�AĴ�p
     * Phone, Datetime, Number, Email, Url, TextEdit..
     * �᭱�٦��b�B�J�A�N�O�u��������J�A��̫᭱���²��
     * �]�t����ୱ�B��J�k�B�]�w���򪺡A���|�I�s��!!!!
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
     * ��J�k�����ɷ|�I�s��A�Ҧp�i�J�ݾ�
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
     * Step 6.1, �u����J�b��
     *  Implementation of KeyboardViewListener
     *  �ƹ�W�A�� COM Port ��J�]������@�b���A�ӬO��...
     *    getCurrentInputConnection().commitText()
     */
    public void onKey(int primaryCode, int[] keyCodes) {
		Toast.makeText(this, "Enter \"Setup\" -> \"Language & Input\"\n to configure the COM Port" ,
				Toast.LENGTH_LONG).show();
    }

    /*
     * Step 6.2, other implements
     * ���U�u�O���n����@�A�@����J�k�A�z�ױ���������L�A���M�ڭ̤��ݭn
     */
    // onText() �n���O�� Candidate �i�H��J��ꪺ�A�ڭ̥Τ���
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