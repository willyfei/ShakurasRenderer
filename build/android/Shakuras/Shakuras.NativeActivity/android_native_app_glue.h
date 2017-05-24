/*
 * ��Ȩ���� (C) 2010 Android ����Դ������Ŀ
 *
 * ���� Apache ��� 2.0 �汾(��Ϊ����ɡ�)������ɣ�
 * Ҫʹ�ô��ļ���������ѭ����ɡ��е�˵����
 * ����Դ�����λ�û�ȡ����ɡ��ĸ���
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * �������÷���Ҫ�������ͬ�⣬����
 * ����ɡ�������������ԭ�������䣬
 * ���ṩ�κ���ʽ(��������ʾ����Ĭʾ)�ĵ�����������
 * �μ�����ɡ��˽⡰��ɡ��й���Ȩ�޺�
 * ���Ƶ�ָ�����ԡ�
 *
 */

#ifndef _ANDROID_NATIVE_APP_GLUE_H
#define _ANDROID_NATIVE_APP_GLUE_H

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * �� <android/native_activity.h> �ṩ�ı��ػ����
 * ����һ��Ӧ�ó����ṩ�Ļص���
 * ����ĳЩ�¼�ʱ��������߳̽�������Щ�ص���
 *
 * ����ζ�ţ���Щ�ص��е�ÿ���ص���Ӧ�������������ϵͳ
 * ǿ�ƹر�Ӧ�ó���ķ��ա��˱��
 * ģ��ֱ�ӡ����ͣ�������Լ����
 *
 *��threaded_native_app����̬�������ṩ��ͬ��
 * ִ��ģ�ͣ�����Ӧ�ó����������
 * �߳���ʵʩ���Լ�����Ҫ�¼�ѭ�����乤����ʽ����:
 *
 * 1/ Ӧ�ó�������ṩ��Ϊ��android_main()���ĺ�����
 *    ���ڲ�ͬ�ڻ�����̵߳�
 *    ���߳��д����ʱ�������øú�����
 *
 * 2/ android_main() ���յ�ָ����Ч�ġ�android_app���ṹ��ָ�룬
 *    �ýṹ������������Ҫ����(���磬
 *    Ӧ�ó��������������е� ANativeActivity ����ʵ��)�����á�
 *
 * 3/��android_app���������Ѿ�
 *    ���������������ش��¼��� ALooper ʵ��:
 *
 *      - ����������¼�(���硰��ͣ�������ָ���)���μ����ĵ� APP_CMD_XXX
 *        ������
 *
 *      - ���Ը��ӵ���� AInputQueue �������¼���
 *
 *    ÿ���¼�����Ӧ��һ����ֵ�ֱ�Ϊ LOOPER_ID_MAIN �� LOOPER_ID_INPUT ��
 *    ALooper_pollOnce ���ص� ALooper
 *    ��ʶ����
 *
 *    ���Ӧ�ó����ʹ����ͬ�� ALooper ��������
 *    �ļ������������ǿɻ��ڻص��������
 *    LOOPER_ID_USER ��ͷ�ķ��ر�ʶ����
 *
 * 4/ �����ں�ʱ�յ� LOOPER_ID_MAIN �� LOOPER_ID_INPUT �¼���
 *    ���ص����ݶ���ָ�� android_poll_source �ṹ����
 *    �ɶ������� process() ������������Ҫ���õ� android_app->onAppCmd
 *    �� android_app->onInputEvent���Դ������Լ���
 *    �¼���
 *
 *    ���⣬�㻹�ɵ��õͼ�����ֱ��
 *    ��ȡ�ʹ�������...  �鿴���ӵ� process_cmd() �� process_input()
 *    ʵ�֣����˽���ν��д˲�����
 *
 * �鿴�������������� NDK ���ṩ����Ϊ 
 *��native-activity����ʾ��������鿴 NativeActivity �� JavaDoc��
 */

struct android_app;

/**
 * ����Դ������׼��������
 * �� ALooper fd ��ص����ݽ���Ϊ��outData�����ء�
 */
struct android_poll_source {
    //��Դ�ı�ʶ����  ������ LOOPER_ID_MAIN ��
    // LOOPER_ID_INPUT��
    int32_t id;

    //�� ident ������ android_app��
    struct android_app* app;

    //Ҫ�Ӵ�Դ������ִ�б�׼���ݴ����
    //������
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

/**
 * ��������̻߳�Ӧ�ó���ı�׼ճ�������
 * ���档 �ڴ�ģ���У�Ӧ�ó���Ĵ�����
 * ���Լ����߳������У����̶߳����ڽ��̵����̡߳�
 * ���̲߳����� Java
 * VM ����������������Ҫ�໥������ʹ JNI �����κ�
 *  Java ����
 */
struct android_app {
    //���Ը�⣬Ӧ�ó���ɽ�ָ�����Լ���״̬�����ָ��
    //�������
    void* userData;

    //�ڴ˴�������ڴ�����Ӧ������(APP_CMD_*)�ĺ���
    void (*onAppCmd)(struct android_app* app, int32_t cmd);

    //�ڴ˴�������ڴ��������¼��ĺ�������ʱ��
    //�¼��Ѿ���ǰ���ɣ�һ�����ؽ�����
    //��ɡ�������Ѵ�����¼����򷵻� 1�������κ�Ĭ��
    //���ɷ��� 0��
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);

    //��Ӧ�����������е� ANativeActivity ����ʵ����
    ANativeActivity* activity;

    //�������е�Ӧ�ò��õĵ�ǰ���á�
    AConfiguration* config;

    //���Ǵ���ʱ�ṩ����һ��ʵ�����ѱ���״̬��
    //���û��״̬����Ϊ NULL����ɸ�����Ҫʹ�ã�
    //�ڴ潫���ֿ��ã�ֱ����Ϊ APP_CMD_RESUME ���� android_app_exec_cmd()��
    //��ʱ�����ͷ��ڴ棬���� savedState ����Ϊ NULL��
    //�������� APP_CMD_SAVE_STATE ʱ�ſɸ�����Щ������
    //��ʱ����Щ��������ʼ��Ϊ NULL���������ܹ�Ϊ���
    //״̬�����ڴ沢����Ϣ���ڴ˴�����������£����Ժ�
    //Ϊ���ͷ��ڴ档
    void* savedState;
    size_t savedStateSize;

    //��Ӧ�õ��̹߳����� ALooper��
    ALooper* looper;

    //��Ϊ NULL ʱ������һ��������У�Ӧ�ý�ͨ����
    //�����û��������С�
    AInputQueue* inputQueue;

    //��Ϊ NIULL ʱ������Ӧ�ÿ������н��л��ƵĴ��ڽ��档
    ANativeWindow* window;

    //���ڵĵ�ǰ���ݾ��Σ�����
    //Ӧ���ô��ڵ������Թ��û��鿴������
    ARect contentRect;

    //Ӧ�õĻ�ĵ�ǰ״̬�������� APP_CMD_START��
    //APP_CMD_RESUME��APP_CMD_PAUSE �� APP_CMD_STOP����μ����ġ�
    int activityState;

    //Ӧ�ó���� NativeActivity ���ƻ���������
    //�ȴ�Ӧ���߳����ʱ����Ϊ����ֵ��
    int destroyRequested;

    // -------------------------------------------------
    //������ճ������ġ�˽�С�ʵ�֡�

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int msgread;
    int msgwrite;

    pthread_t thread;

    struct android_poll_source cmdPollSource;
    struct android_poll_source inputPollSource;

    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue* pendingInputQueue;
    ANativeWindow* pendingWindow;
    ARect pendingContentRect;
};

enum {
    /**
     *����� Looper ���� ID�����е���������Ӧ�õ����̣߳������� ID ��Ϊһ����ʶ����
     *ALooper_pollOnce() ���ء�  ��
     * ��ʶ����������ָ�� android_poll_source structure �ṹ��ָ�롣
     * ��ʹ�� android_app_read_cmd()
     * �� android_app_exec_cmd() �����ʹ�����Щ���ݡ�
     */
    LOOPER_ID_MAIN = 1,

    /**
     * �¼��� Looper ���� ID�����е��¼�����Ӧ�ó��򴰿ڵ� AInputQueue��
     * ������ ID ��Ϊһ����ʶ����
     * ALooper_pollOnce() ���ء��˱�ʶ����������ָ��
     * android_poll_source �ṹ��ָ�롣��Щ���ݿ�ͨ��
     *android_app �� inputQueue �����ȡ��
     */
    LOOPER_ID_INPUT = 2,

    /**
     * �û������ ALooper ��ʶ���Ŀ�ͷ��
     */
    LOOPER_ID_USER = 3,
};

enum {
    /**
     * �������̵߳�����: AInputQueue �Ѹ��ġ�  ����
     * ������ʱ��android_app->inputQueue �����µ��µĶ���
     * (�� NULL)��
     */
    APP_CMD_INPUT_CHANGED,

    /**
     * �������̵߳�����: �µ� ANativeWindow ��׼�������ɹ�ʹ�á�  ���յ�
     * �������android_app->window �������µĴ���
     * ���档
     */
    APP_CMD_INIT_WINDOW,

    /**
     * �������̵߳�����: ��Ҫ��ֹ
     * Ŀǰ�� ANativeWindow��  �յ��������android_app->window ��
     * �������д��ڣ����� android_app_exec_cmd ��
     * ����������Ϊ NULL��
     */
    APP_CMD_TERM_WINDOW,

    /**
     * �������̵߳�����: �ѵ�����ǰ ANativeWindow �Ĵ�С��
     * ����������´�С���»��ơ�
     */
    APP_CMD_WINDOW_RESIZED,

    /**
     * �������̵߳�����: ϵͳ��Ҫ���»���
     * ��ǰ ANativeWindow��  ��Ӧ���ڽ�����ش���֮ǰ���������»��Ƶ�
     * android_app_exec_cmd()���Ա�����ݵĻ��ƹ��ϡ�
     */
    APP_CMD_WINDOW_REDRAW_NEEDED,

    /**
     *�������̵߳�����: ���ڵ����������Ѹ��ģ�
     * ���磬����ʾ�����ص������봰�ڸ���Ϊ��������  �����
     *android_app::contentRect ���ҵ��µ����ݾ��Ρ�
     */
    APP_CMD_CONTENT_RECT_CHANGED,

    /**
     * �������̵߳�����: Ӧ�õĻ�����ѻ��
     * ���뽹�㡣
     */
    APP_CMD_GAINED_FOCUS,

    /**
     * �������̵߳�����: Ӧ�õĻ�����Ѷ�ʧ
     *���뽹�㡣
     */
    APP_CMD_LOST_FOCUS,

    /**
     * �������̵߳�����: ��ǰ�豸�����Ѹ��ġ�
     */
    APP_CMD_CONFIG_CHANGED,

    /**
     * �������̵߳�����: ϵͳ������ʱ�ڴ治�㡣
     * ���Լ�������ڴ�ʹ�á�
     */
    APP_CMD_LOW_MEMORY,

    /**
     * �������̵߳�����: Ӧ�õĻ��������
     */
    APP_CMD_START,

    /**
     * �������̵߳�����: Ӧ�õĻ�ѻָ���
     */
    APP_CMD_RESUME,

    /**
     * �������̵߳�����: Ӧ��ӦΪ���Լ������µ��ѱ���״̬��
     * �Ա�������Ҫʱ������л�ԭ��  ��������ѱ���״̬��
     * ͨ�������ڴ������з��䲢������
     * android_app.savedStateSize �еĴ�С���� android_app.savedState �С�  �������Ժ�Ϊ��
     * �ͷ��ڴ档
     */
    APP_CMD_SAVE_STATE,

    /**
     * �������̵߳�����: Ӧ�õĻ����ͣ��
     */
    APP_CMD_PAUSE,

    /**
     * �������̵߳�����: Ӧ�õĻ��ֹͣ��
     */
    APP_CMD_STOP,

    /**
     * �������̵߳�����: Ӧ�õĻ�ѱ��ƻ���
     * ���ڵȴ�Ӧ�ó������˳���Ȼ�������
     */
    APP_CMD_DESTROY,
};

/**
 * ALooper_pollAll() ���� LOOPER_ID_MAIN ʱ���ã���ȡ��һ
 * Ӧ��������Ϣ��
 */
int8_t android_app_read_cmd(struct android_app* android_app);

/**
 * �� android_app_read_cmd() ���ص�������ã��Զ�
 * ������������г�ʼԤ�������ô˺�������ɶ�
 * ����ִ�����Լ��Ĳ�����
 */
void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * �� android_app_read_cmd() ���ص�������ã��Զ�
 * ����������������պ������ô˺���ǰ�������ȶ�
 * ����ִ�����Լ��Ĳ�����
 */
void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd);

/**
 * ����Ӧ�ó���������ʵ�ֵĺ�������ʾ
 * Ӧ�õ��������
 */
extern void android_main(struct android_app* app);

#ifdef __cplusplus
}
#endif

#endif /* _ANDROID_NATIVE_APP_GLUE_H */
