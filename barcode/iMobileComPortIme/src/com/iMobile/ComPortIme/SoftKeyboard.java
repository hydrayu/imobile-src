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

/**
 * Example of writing an input method for a soft keyboard.  This code is
 * focused on simplicity over completeness, so it should in no way be considered
 * to be a complete soft keyboard implementation.  Its purpose is to provide
 * a basic example for how you would get started writing an input method, to
 * be fleshed out as appropriate.
 */
public class SoftKeyboard extends InputMethodService 
        implements KeyboardView.OnKeyboardActionListener {
    static final boolean DEBUG = false;
    private KeyboardView mInputView;
    private Keyboard mComPortImeKeyboard;
    
    /**
     * Step 1, onCreate()
     * Main initialization of the input method component.  Be sure to call
     * to super class.
     */
    @Override public void onCreate() {
        super.onCreate();
    }
    
    /**
     * Step 1.1, onInitializeInterface()
     * This is the point where you can do all of your UI initialization.  It
     * is called after creation and any configuration change.
     */
    @Override public void onInitializeInterface() {
    	mComPortImeKeyboard = new Keyboard(this, R.xml.comime);
    }
    
    /**
     * Step 2. onCreateInputView()
     * Designing the Input Method UI
     * Called by the framework when your view for creating input needs to
     * be generated.  This will be called the first time your input method
     * is displayed, and every time it needs to be re-created such as due to
     * a configuration change.
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
     * Step 4, onStartInputView()
     * When an input field receives focus and your IME starts, 
     * the system calls onStartInputView()
     */
    @Override public void onStartInputView(EditorInfo attribute, boolean restarting) {
        super.onStartInputView(attribute, restarting);
        // Apply the selected keyboard to the input view.
        mInputView.setKeyboard(mComPortImeKeyboard);
        mInputView.closing();
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
     */
    @Override public void onStartInput(EditorInfo attribute, boolean restarting) {
        super.onStartInput(attribute, restarting);
    }

    /*
     * Step 7, onCurrentInputMethodSubtypeChanged()
     * Skip it, we don't support subtype
     */
    
    /**
     * Step 8, onFinishInput()
     * This is called when the user is done editing a field.  We can use
     * this to reset our state.
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
    
    /*
     * Step 6.1, 真正輸入在此
     *  Implementation of KeyboardViewListener
     *  事實上，有 COM Port 輸入也不必實作在此，而是用...
     *    getCurrentInputConnection().commitText()
     */
    public void onKey(int primaryCode, int[] keyCodes) {
        // Sending Text to Application
//        getCurrentInputConnection().commitText(
//                String.valueOf((char) primaryCode), 1);
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
}