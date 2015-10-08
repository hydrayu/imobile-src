This APP can demo a JNI example
1. Java area call function, then through JNI return string(HelloWorld)
2. Java area send a string, then through JNI return string
3. Java area send a int, then through JNI return integer
4. Java area send a IntArray, then through JNI (Array + 5), last return to Java area
5. Java area send a boolean, then through JNI return (!boolean) to Java area  

Reference:
1. http://fecbob.pixnet.net/blog/post/36298824---android%E4%BD%BF%E7%94%A8jni%E5%AF%A6%E7%8F%BEjava%E8%88%87c%E4%B9%8B%E9%96%93%E5%82%B3%E9%81%9E%E8%B3%87%E6%96%99(%E8%BD%89)
2. https://github.com/songzhiyong/lib_song/blob/master/jni/mydecode.c
3. http://stackoverflow.com/questions/8133074/error-unknown-type-name-bool
4. http://www.cnblogs.com/kchen/archive/2011/08/03/android_ndk_using_jni.html
5. http://nano-chicken.blogspot.tw/2011/01/linux-modules11module-parameters.html
6. http://pydoing.blogspot.tw/2010/07/c-strtok.html
7. http://blog.wu-boy.com/2010/08/cc-cstring-string-h-%E5%87%BD%E5%BC%8F%EF%BC%9Astrcat-strncat-strcmp-strncmp/
8. http://jashliao.pixnet.net/blog/post/166464918-%5Bc-c%2B%2B%E5%9F%BA%E7%A4%8E%5D-%E7%B4%94c%E5%AD%97%E4%B8%B2%E6%AF%94%E5%B0%8D%EF%BC%88%E6%AF%94%E8%BC%83%EF%BC%89%E5%87%BD%E6%95%B8%E4%BB%8B%E7%B4%B9%E7%AF%84%E4%BE%8B
9. http://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings

result:
![alt tag](https://github.com/showoowohs/Po_git/blob/PoTestJNI/PoTestJNI/result.png)