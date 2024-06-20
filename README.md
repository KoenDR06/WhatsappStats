# About
This repository is a program which reads the entire WhatsApp history of a group or personal chat and gives you stats
about that chat.

# Features
### What's supported
- Average time between messages
- Total message count
- Who sent how many messages
- Group chats
- Average word count per message
- Word count
- Most used words
- Locations
- Polls

### What's not supported yet
- Viewing media
- Conversation detection
- Contacts
- Pings
- Live locations
- Message formats that are not `M`/`DD`/`YY`, `HH`:`MM` - `SENDER`: `MESSAGE`

### What's not possible to support
- Replies. This is simply because the 'Export Chat' option does not write down if the message replies to something, 
there's nothing I can do about that.


# Usage
## Android
If you want to export a chat in WhatsApp, you first have to open a chat. When you have opened the chat, tap on the three
dots in the upper right corner of your screen. You will see a menu. Click on more, and then on "Export Chat".
Choose "without media", and you get to choose where the zip file goes. Unzip the file, rename it to chat.txt and put it
in the `/chat` directory. The program is now ready to run.

Below is a screenshot of the steps you need to take.

<img src="assets/android_usage_0.jpg" width="500" alt="How to export chat on Android devices"/>

## iOS
If you want to export a chat in WhatsApp, you first have to open a chat. When you have opened the chat, tap on the
contact / group name. Scroll down until you see the "Export Chat" option. After you click on it, choose "without media",
and you get to choose where the zip file goes. Unzip the file, rename it to chat.txt and put it in the `/chat`
directory. The program is now ready to run.

Below is a screenshot of the steps you need to take.

<img src="assets/ios_usage_0.png" width="500" alt="How to export chat on iOS devices"/>