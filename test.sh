
HAS_PROTOC=`which protoc > /dev/null && echo true || echo false`

HAS_VALID_PROTOC=`echo false`
HAS_PLUGIN=`echo false`

if [ ${HAS_PROTOC} ]
then
	HAS_VALID_PROTOC=`protoc --version | grep -q libprotoc.3 2> /dev/null && echo true || echo false`
fi

HAS_PLUGIN=`which grpc_cpp_plugin > /dev/null && echo true || echo false`


SYSTEM_OK=false

if [ ${HAS_VALID_PROTOC} ]
then
	if [ ${HAS_PLUGIN} ]
	then
		SYSTEM_OK=true
	fi
fi



echo  ${HAS_PROTOC} ${HAS_VALID_PROTOC} ${HAS_PLUGIN} ${SYSTEM_OK}

