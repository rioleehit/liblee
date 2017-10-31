# liblee
# 我自己写的c++库 
# 希望以后会有新的功能


# 目前 有两个功能
#  get/set ———— 使用lambda function 实现 可以隐藏类的成员变量 
#  switch case ———— 使用 lambda function initializer_list 实现 可以 witch 自定义类型（需要重载==相等比较函数）


#  增加 新功能 宽窄字符转换（utf-8对应的char  跟wchart_t）
#  增加 新功能 handle回调（制作中）
#  增加 新功能 xml解析 （制作中）
	————仅支持部分解析 
	————特定格式设计  <tagName=""/> 
	————<... >标签中间的文本</...> 标签中间的文本 将被忽略 
	————设计增加 json 解析； 可以让 json 跟 xml 互转 （ 统一使用  Object 保存数据）

 

