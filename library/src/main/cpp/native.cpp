//
// Created by 李樟取 on 2017/5/25.
//
#include <curl/curl.h>
#include "native.h"
#include "log.h"
#include "iostream"
#include <sstream>
#include <cmath>
#include "string"
#include "vector"
#include "curl/curl.h"

static std::string formatJson(char *json) {
    std::string pretty;

    if (json == NULL || strlen(json) == 0) {
        return pretty;
    }

    std::string str = std::string(json);
    bool quoted = false;
    bool escaped = false;
    std::string INDENT = "    ";
    int indent = 0;
    int length = (int) str.length();
    int i;

    for (i = 0; i < length; i++) {
        char ch = str[i];

        switch (ch) {
            case '{':
            case '[':
                pretty += ch;

                if (!quoted) {
                    pretty += "\n";

                    if (!(str[i + 1] == '}' || str[i + 1] == ']')) {
                        ++indent;

                        for (int j = 0; j < indent; j++) {
                            pretty += INDENT;
                        }
                    }
                }

                break;

            case '}':
            case ']':
                if (!quoted) {
                    if ((i > 0) && (!(str[i - 1] == '{' || str[i - 1] == '['))) {
                        pretty += "\n";

                        --indent;

                        for (int j = 0; j < indent; j++) {
                            pretty += INDENT;
                        }
                    } else if ((i > 0) && ((str[i - 1] == '[' && ch == ']') ||
                                           (str[i - 1] == '{' && ch == '}'))) {
                        for (int j = 0; j < indent; j++) {
                            pretty += INDENT;
                        }
                    }
                }

                pretty += ch;

                break;

            case '"':
                pretty += ch;
                escaped = false;

                if (i > 0 && str[i - 1] == '\\') {
                    escaped = !escaped;
                }

                if (!escaped) {
                    quoted = !quoted;
                }

                break;

            case ',':
                pretty += ch;

                if (!quoted) {
                    pretty += "\n";

                    for (int j = 0; j < indent; j++) {
                        pretty += INDENT;
                    }
                }

                break;

            case ':':
                pretty += ch;

                if (!quoted) {
                    pretty += " ";
                }

                break;

            default:
                pretty += ch;

                break;
        }
    }

    return pretty;
}


static size_t header_callback(char *buffer, size_t size,
                              size_t nitems, void *userdata) {
    ALOGE("Header:%s", buffer);
    return nitems * size;
}

static size_t write_callback(char *buffer, size_t size, size_t nmemb, void *userdata) {
//    const std::string &string = formatJson(buffer);
    ALOGE("%s", buffer);
    return nmemb * size;
}

void dump(JNIEnv *env, jobject thiz, jstring jurl) {
    char *url = (char *) env->GetStringUTFChars(jurl, NULL);
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10); // 10 s connect timeout
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        if (CURLE_OK == res) {
            ALOGE("success send request: %s", url);
        } else {
            const char *easy_error = curl_easy_strerror(res);
            ALOGE("success send request :%s . error code %d. error msg: %s", url, res, easy_error);
        }
        curl_easy_cleanup(curl);
    }
    env->ReleaseStringUTFChars(jurl, url);
}

void test(JNIEnv *env, jobject thiz) {

    char *string = curl_version();

    ALOGE("curl_version:%s", string);

    curl_version_info_data *p = curl_version_info(CURLVERSION_NOW);
    const char *const *prot = p->protocols;

    unsigned int i = 0;
    std::vector<std::string> protocols;
    while (*(prot + i) != NULL) {
        protocols.push_back(std::string(*(prot + i)));
        i++;
    }

    ALOGE("protocols num = %d", protocols.size());
    for (std::string p:protocols) {
        ALOGE("protocol %s", p.c_str());
    }

    int features = p->features;
    ALOGE("CURL_VERSION_IPV6 = %d", (features & CURL_VERSION_IPV6) == CURL_VERSION_IPV6);
    ALOGE("CURL_VERSION_KERBEROS4 = %d",
          (features & CURL_VERSION_KERBEROS4) == CURL_VERSION_KERBEROS4);
    ALOGE("CURL_VERSION_SSL = %d", (features & CURL_VERSION_SSL) == CURL_VERSION_SSL);
    ALOGE("CURL_VERSION_LIBZ = %d", (features & CURL_VERSION_LIBZ) == CURL_VERSION_LIBZ);
    ALOGE("CURL_VERSION_NTLM = %d", (features & CURL_VERSION_NTLM) == CURL_VERSION_NTLM);
    ALOGE("CURL_VERSION_GSSNEGOTIATE = %d",
          (features & CURL_VERSION_GSSNEGOTIATE) == CURL_VERSION_GSSNEGOTIATE);
    ALOGE("CURL_VERSION_DEBUG = %d", (features & CURL_VERSION_DEBUG) == CURL_VERSION_DEBUG);
    ALOGE("CURL_VERSION_ASYNCHDNS = %d",
          (features & CURL_VERSION_ASYNCHDNS) == CURL_VERSION_ASYNCHDNS);
    ALOGE("CURL_VERSION_SPNEGO = %d", (features & CURL_VERSION_SPNEGO) == CURL_VERSION_SPNEGO);
    ALOGE("CURL_VERSION_LARGEFILE = %d",
          (features & CURL_VERSION_LARGEFILE) == CURL_VERSION_LARGEFILE);
    ALOGE("CURL_VERSION_IDN = %d", (features & CURL_VERSION_IDN) == CURL_VERSION_IDN);
    ALOGE("CURL_VERSION_SSPI = %d", (features & CURL_VERSION_SSPI) == CURL_VERSION_SSPI);
    ALOGE("CURL_VERSION_CONV = %d", (features & CURL_VERSION_CONV) == CURL_VERSION_CONV);
    ALOGE("CURL_VERSION_CURLDEBUG = %d",
          (features & CURL_VERSION_CURLDEBUG) == CURL_VERSION_CURLDEBUG);
    ALOGE("CURL_VERSION_TLSAUTH_SRP = %d",
          (features & CURL_VERSION_TLSAUTH_SRP) == CURL_VERSION_TLSAUTH_SRP);
    ALOGE("CURL_VERSION_NTLM_WB = %d", (features & CURL_VERSION_NTLM_WB) == CURL_VERSION_NTLM_WB);
    ALOGE("CURL_VERSION_HTTP2 = %d", (features & CURL_VERSION_HTTP2) == CURL_VERSION_HTTP2);
    ALOGE("CURL_VERSION_GSSAPI = %d", (features & CURL_VERSION_GSSAPI) == CURL_VERSION_GSSAPI);
    ALOGE("CURL_VERSION_KERBEROS5 = %d",
          (features & CURL_VERSION_KERBEROS5) == CURL_VERSION_KERBEROS5);
    ALOGE("CURL_VERSION_UNIX_SOCKETS = %d",
          (features & CURL_VERSION_UNIX_SOCKETS) == CURL_VERSION_UNIX_SOCKETS);
    ALOGE("CURL_VERSION_PSL = %d", (features & CURL_VERSION_PSL) == CURL_VERSION_PSL);
    ALOGE("CURL_VERSION_HTTPS_PROXY = %d",
          (features & CURL_VERSION_HTTPS_PROXY) == CURL_VERSION_HTTPS_PROXY);


    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10); // 10 s connect timeout
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://nghttp2.org");
        res = curl_easy_perform(curl);
        if (CURLE_OK == res) {
            char *ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == res) && ct)
                ALOGE("Content-Type: %s\n", ct);
            long http_version_code;;

            res = curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &http_version_code);
            if ((CURLE_OK == res) && http_version_code) {
                if (http_version_code == CURL_HTTP_VERSION_NONE) {
                    ALOGE("Unknown");
                } else if (http_version_code == CURL_HTTP_VERSION_1_0) {
                    ALOGE("HTTP/1.0");
                } else if (http_version_code == CURL_HTTP_VERSION_1_1) {
                    ALOGE("HTTP/1.1");
                } else if (http_version_code == CURL_HTTP_VERSION_2_0) {
                    ALOGE("HTTP/2.0");
                } else if (http_version_code == CURL_HTTP_VERSION_2TLS) {
                    ALOGE("HTTP/2.0 For Https, Http/1.1 For Http");
                } else if (http_version_code == CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE) {
                    ALOGE("please use HTTP 2 without HTTP/1.1 Upgrade");
                } else if (http_version_code == CURL_HTTP_VERSION_LAST) {
                    ALOGE("ILLEGAL* http version");
                }


            }
        } else {
            const char *easy_error = curl_easy_strerror(res);
            std::cout << easy_error << std::endl;
            ALOGE("error code %d  msg: %s", res, easy_error);
        }
        curl_easy_cleanup(curl);
    }


}

static const JNINativeMethod sMethods[] = {
        {
                const_cast<char *>("test"),
                const_cast<char *>("()V"),
                reinterpret_cast<void *>(test)
        },
        {
                const_cast<char *>("dump"),
                const_cast<char *>("(Ljava/lang/String;)V"),
                reinterpret_cast<void *>(dump)
        },


};

int registerNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *methods,
                          const int numMethods) {
    jclass clazz = env->FindClass(className);
    if (!clazz) {
        ALOGE("Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, numMethods) != 0) {
        ALOGE("RegisterNatives failed for '%s'\n", className);
        env->DeleteLocalRef(clazz);
        return JNI_FALSE;
    }
    env->DeleteLocalRef(clazz);
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    registerNativeMethods(env, CLASSNAME, sMethods, NELEM(sMethods));
    return JNI_VERSION_1_6;
}