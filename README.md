服务器命令格式

|名称|功能码/quint8|宏定义|命令字/quint8|宏定义|备注|
|:------|:------------------:|:--------|:--------:|:--------|:---:|
|重启|0x55|send_type_msg|0x00|send_type_reboot|---|
|-
|获取IP地址|0x55|send_type_msg|0x01|send_type_ip|---|
|获取出厂编号|0x55|send_type_msg|0x02|send_type_No|---|
|获取MAC地址|0x55|send_type_msg|0x03|send_type_mac|---|
|获取版本|0x55|send_type_msg|0x04|send_type_version|---|
|发送心跳|0x55|send_type_msg|0x05|send_type_heart|---|
|-|
|启动测试|0x55|send_type_msg|0x10|send_type_test|---|
|获取状态|0x55|send_type_msg|0x11|send_type_state|---|
|获取配置|0x55|send_type_msg|0x12|send_type_config|---|
|开启结果上传|0x55|send_type_msg|0x13|send_type_network|---|
|关闭结果上传|0x55|send_type_msg|0x13|send_type_network|---|
|-
|发送文件|0x56|send_type_file|

二代机应答格式

|名称|功能码/quint8|宏定义|命令字/quint8|宏定义|数据|备注|
|:------|:------------------:|:--------|:------------------:|:---------|:----:|:----:|
|回传IP地址|0x80|reply_type_msg|0x81|reply_type_ip|data|---|
|回传出厂编号|0x80|reply_type_msg|0x82|reply_type_No|data|---|
|回传MAC地址|0x80|reply_type_msg|0x83|reply_type_mac|data|---|
|回传版本|0x80|reply_type_msg|0x84|reply_type_version|data|---|
|回传心跳|0x80|reply_type_msg|0x85|reply_type_heart|data|---|
|-
|回传状态|0x80|reply_type_msg|0x90|reply_type_test|data|---|
|回传状态|0x80|reply_type_msg|0x91|reply_type_state|data|---|
|回传配置|0x80|reply_type_msg|0x92|reply_type_config|data|---|
|回传结果|0x80|reply_type_msg|0x93|reply_type_result|data|---|

客户端应答格式

|名称|功能码/quint8|宏定义|命令字/quint8|宏定义|数据|备注|
|:------|:------------------:|:--------|:------------------:|:---------|:----:|:----:|
|回传IP地址|0x80|reply_type_msg|0x81|reply_type_ip|data|---|
|回传出厂编号|0x80|reply_type_msg|0x82|reply_type_No|data|---|
|回传MAC地址|0x80|reply_type_msg|0x83|reply_type_mac|data|---|
|回传版本|0x80|reply_type_msg|0x84|reply_type_version|data|---|
|回传心跳|0x80|reply_type_msg|0x85|reply_type_heart|data|---|
|-
|回传状态|0x80|reply_type_msg|0x90|reply_type_test|data|---|
|回传状态|0x80|reply_type_msg|0x91|reply_type_state|data|---|
