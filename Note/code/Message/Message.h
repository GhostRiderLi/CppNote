/*
此文件是Message部分
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <set>

class Folder;

class Message{
    friend class Folder;
    friend void swap(Message &lhs, Message &rhs);
public:
    explicit Message(const std::string &str = std::string() ):contents(str){ }
    Message(const Message& msg);
    //没有定义为noexcpet，因为移动Folders会分配空间，有可能产生异常
    Message(Message&& msg); 
    Message& operator=(const Message&);
    //没有定义为noexcpet，因为移动Folders会分配空间，有可能产生异常
    Message& operator=(Message&&);
    ~Message();

    //将本消息Message添加到指定Folder
    void save(Folder&);
    //将本消息Message从指定Folder删除
    void remove(Folder&);

private:
    std::string contents;
    std::set<Folder*> folders; //folders including message

    inline std::string message() const {return contents;}
    //将本Message 添加到参数的Folder中
    inline void add_to_folders(const Message&);
    //将本Message从folders中每个Folder删除
    inline void remove_from_foders();

    inline void insert_to_folders(Folder* folder){folders.insert(folder);}

    inline void erase_from_folders(Folder* folder){folders.erase(folder);}

    void move_Folders(Message* m);
};


class Folder{
    friend class Message;
    friend void print(std::ostream& os, Folder& folder);
public:
    explicit Folder(std::set<Message*> msg = std::set<Message*>()):messages(msg){ }
    Folder(const Folder& folder):messages(folder.messages){add_to_msgs();}
    ~Folder(){ remove_from_msgs();}
    Folder& operator=(const Folder&);

    void addMsg(Message* msg);
    void remMsg(Message* msg);
private:
    std::set<Message*> messages;

    inline std::string get_message(Message* pmsg) const {return pmsg->message();}
    //在本目录中增加消息，不更新消息msg信息
    inline void add_msg(Message* msg){messages.insert(msg);}
    //在本目录中移除消息，不更新消息msg信息
    inline void rem_msg(Message* msg){messages.erase(msg);}
    //从本目录中的消息的记录中移除本目录
    inline void remove_from_msgs(){for(auto pmsg: messages){pmsg->erase_from_folders(this);}}
    //从本目录中的消息的记录中添加本目录
    inline void add_to_msgs(){for(auto pmsg: messages){pmsg->insert_to_folders(this);}}

};

/* Meaaage 成员的定义 START */

inline void Message::add_to_folders(const Message& msg){
    for(auto pfolder : msg.folders){pfolder->add_msg(this);}
}

inline void Message::remove_from_foders(){
    for(auto pfolder : folders){pfolder->rem_msg(this);}
}

void Message::move_Folders(Message* m){
    folders = std::move(m->folders);
    for(auto f : folders){
        f->rem_msg(m);
        f->add_msg(this);
    }
    m->folsers.clear();
}

Message::Message(const Message& msg):\
                contents(msg.contents),folders(msg.folders){
    add_to_folders(msg);
}

Message::Message(Message&& msg):contents(std::move(msg.contents)){
    move_Folders(&msg);
}

Message& Message::operator=(const Message& msg){
    if(this != &msg){
        remove_from_foders();
        contents = msg.contents;
        folders = msg.folders;
        add_to_folders(msg);
    }
    return *this;
}

Message& Message::operator=(Message&& msg){
    if(this != &msg){
        remove_from_foders();
        contents = std::move(msg.contents);
        move_Folders(&msg);
    }

    return *this;
}
Message::~Message(){
    remove_from_foders();
}

void Message::save(Folder& folder){
    folders.insert(&folder);
    folder.add_msg(this);
}

void Message::remove(Folder& folder){
    folders.erase(&folder);
    folder.rem_msg(this);
}

void swap(Message &lhs, Message &rhs){
    using std::swap;
    for(auto f: lhs.folders){f->rem_msg(&lhs);}
    for(auto f: rhs.folders){f->rem_msg(&rhs);}
    swap(lhs.contents, rhs.contents);
    swap(lhs.folders, rhs.folders);
    for(auto f: lhs.folders){f->add_msg(&lhs);}
    for(auto f: rhs.folders){f->add_msg(&rhs);}
}
/* Meaaage 成员的定义 END */ 


/*Folder 成员定义 START*/


Folder& Folder::operator=(const Folder& folder){
    if(this != &folder){
        remove_from_msgs();
        messages = folder.messages;
        add_to_msgs();
    }
    return *this;
}

void Folder::addMsg(Message* msg){
    add_msg(msg);
    msg->folders.insert(this);
}

void Folder::remMsg(Message* msg){
    rem_msg(msg);
    msg->folders.erase(this);
}

void print(std::ostream& os, Folder& folder){
    for(auto pmsg: folder.messages){
        os << folder.get_message(pmsg) << " ";
    }
    os << "\n";
}
/*Folder 成员定义 END*/
#endif //end #ifndef MESSAGE_H