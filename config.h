#ifndef CONFIG_H
#define CONFIG_H

#include <QtWidgets>
#define LOCAL_IP    QHostAddress::LocalHost
#define SERVER_IP   "192.168.2.27"
#define UDP_PORT        9999


#define A9_SERVER_IP "192.168.43.43"
#define TCP_PORT        6666

/* 数据库查询结构体
 * type 查询的表名
 * column 查询的字段
 * values 查询的字段
 *
 * */
struct select{
    char table_name[16];
    char column[16];
    char values[16];
};

/*信息结构体*/
struct __attribute__ ((__packed__)) message_user{
    char buf[32];//消息指令结构体
    char sqlbuf[3096];
    int nrow;//数据库函数执行后得到的记录的数目
    int ncolumn;//数据库函数执行得到字段的数目
};

/* UDP报头 报尾 */
struct __attribute__ ((__packed__)) info{
     long long size;
     //long size;
     int type;
     int count;
};

#define TRAN_SIZE 1024;
struct picHeadMsg {
    int funCode;                        //!功能码
    unsigned int uTransFrameHeadSize;    //!sizeof(WIFI_FRAME_HEADER)
    unsigned int uTransFrameSize;       //!sizeof(WIFI_FRAME_HEADER) + Data Size

    //数据帧变量
    unsigned int uDataFrameSize;        //数据帧的总大小
    unsigned int uDataFrameTotal;       //一帧数据被分成传输帧的个数
    unsigned int uDataFrameCurr;        //数据帧当前的帧号
    unsigned int uDataInFrameOffset;    //数据帧在整帧的偏移
};


#endif // CONFIG_H
