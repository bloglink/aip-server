服务器命令格式

|名称|功能码/quint8|宏定义|命令字/QByteArray|宏定义|备注|
|:-----:|:-------:|:--------:|:--------:|:--------|:---:|
|重启|55|send_type_msg|0|send_type_reboot|---|
|获取IP地址|55|send_type_msg|1|send_type_ip|---|
|获取出厂编号|55|send_type_msg|2|send_type_No|---|
|获取MAC地址|55|send_type_msg|3|send_type_mac|---|
|启动测试|55|send_type_msg|4|send_type_test|---|
|发送心跳|55|send_type_msg|5|send_type_heart|---|
|获取状态|55|send_type_msg|6|send_type_state|---|
|获取版本|55|send_type_msg|7|send_type_version|---|
|获取配置|55|send_type_msg|8|send_type_config|---|
|开启结果上传|55|send_type_msg|9|send_type_network|---|
|关闭结果上传|55|send_type_msg|9|send_type_network|---|

二代机应答格式

|名称|功能码/quint8|宏定义|数据/QByteArray|备注|
|:-----:|:--------:|:--------|:---:|:---:|
|回传IP地址|101|reply_type_ip|data|---|
|回传出厂编号|102|reply_type_No|data|---|
|回传MAC地址|103|reply_type_mac|data|---|
|回传心跳|105|reply_type_heart|data|---|
|回传状态|106|reply_type_state|data|---|
|回传版本|107|reply_type_version|data|---|
|回传配置|108|reply_type_config|data|---|
|回传结果|109|reply_type_result|data|---|

客户端应答格式

|名称|功能码/quint8|宏定义|数据/QByteArray|备注|
|:-----:|:--------:|:--------|:---:|:---:|
|回传IP地址|101|reply_type_ip|data|---|
|回传软件编号|102|reply_type_No|data|---|
|回传MAC地址|103|reply_type_mac|data|---|
|回传心跳|105|reply_type_heart|data|---|
|回传状态|106|reply_type_state|data|---|
|回传版本|107|reply_type_version|data|---|
|回传配置|108|reply_type_config|data|---|