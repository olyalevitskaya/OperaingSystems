#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: `basename $0` {send_to}"
  exit -1
fi

send() {
	SEND_TO="$1"
	MESSAGE="$2"
	ACCOUNT_ID=$(purple-send PurpleAccountsGetAllActive|grep -m 1 int32|awk '{print $2}')
	CONVERSATION_ID=$(purple-send PurpleConversationNew int32:1 int32:$ACCOUNT_ID string:$SEND_TO|grep -m 1 int32|awk '{print $2}')
	MESSAGE_ID=$(purple-send PurpleConversationGetImData int32:$CONVERSATION_ID|grep -m 1 int32|awk '{print $2}')
	purple-send PurpleConvImSend int32:$MESSAGE_ID string:"$MESSAGE"
}

inotifywait -m -e modify $HOME | while read line
do
  send $1 "$line"
done
