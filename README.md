数据格式

|名称|byte|出厂编号|版本号|
|:----:|:---:|:--:|:--:|
|心跳包|0x00|A|B|
|数据包|0x01|A|B|
|命令包|0x02|A|B|
|文件包|0x03|A|B|

|名称|数据长度/quint16                                 |数据类型/quint8|功能码/quint8|
|------|----------------------------------------------------|-----------------------|-----------------|
|重启|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_reboot/0|
|IP|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_ip/1|
|No|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_No/2|
|MAC|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_mac/3|
|测试|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_test/4|
|心跳|(quint16)(msg.size() - sizeof(quint16))|type_heart/5||
|状态|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_state/6|
|版本|(quint16)(msg.size() - sizeof(quint16))|type_msg/55|type_version/7|
