package com.imobile.mt8382readkeyevent;


public class imobileJNI {

	static public native String ReadProc(String Read_Path);

	static public native String WriteProc(String Write_Path, String Write_Value);
}
