package io.github.lizhangqu.curl;

/**
 * @version V1.0
 * @author: lizhangqu
 * @date: 2017-05-25 14:17
 */
public class Curl {
    static {
        System.loadLibrary("curl-client");
    }

    public static native void test();

    public static native void dump(String url);
}
