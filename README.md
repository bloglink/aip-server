服务器命令格式

|功能名称解释|字节数|功能码|宏定义        |目标编号|数据|备注|
|----------|:---:|:---:|:------------|:----:|:--:|:-:|
|获取出厂编号|block|1001 |query_num    |X|X||
|获取硬件地址|block|1002 |query_mac    |X|X||
|获取软件版本|block|1003 |query_version|X|X||
|获取目标日期|block|1004 |query_date   |X|X||
|获取目标时间|block|1005 |query_time   |X|X||
|获取目标状态|block|1006 |query_state  |X|X||
|获取目标配置|block|1007 |query_config |X|X||
|
|发送出厂编号|block|2001 |send_num     |X|data||
|发送硬件地址|block|2002 |send_mac     |X|data||
|发送软件版本|block|2003 |send_version |X|data||
|发送目标日期|block|2004 |send_date    |X|data||
|发送目标时间|block|2005 |send_time    |X|data||
|发送目标状态|block|2006 |send_state   |X|data||
|发送目标配置|block|2007 |send_config  |X|data||
|发送文件数据|block|4000 |send_file    |X|data||
|发送文件名称|block|4001 |send_name    |X|data||
|发送文件大小|block|4002 |send_size    |X|data||
|发送校验数据|block|4003 |send_verify  |X|data||
|发送定时心跳|block|4009 |send_heart   |X|X||
|发送测试启动|block|4010 |send_test    |X|X||
|发送错误信息|block|4011 |send_error   |X|data||
|开启结果上传|block|4013 |send_network |X|X||
|关闭结果上传|block|4013 |send_network |X|X|||
