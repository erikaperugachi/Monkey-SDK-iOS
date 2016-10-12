//
//  Monkey.h
//  MonkeyKit
//
//  Created by Gianni Carlo on 6/1/16.
//  Copyright © 2016 Criptext. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MOKMessage.h"
@class MOKConversation;
@class MOKUser;

/**
 *	The `Monkey` class is a singleton through which you can send messages and files and interact with Criptext's servers.
 
 *	@warning The first action of the singleton should always be `initWithApp:secret:user:ignoredParams:expireSession:debugging:autoSync:lastTimestamp:success:failure:`
 */
@interface Monkey : NSObject

/**
 *	Returns the instance of the singleton.
 */
+ (nonnull instancetype)sharedInstance;

/**
 *  String that identifies your App Id.
 */
@property (copy, nonatomic, readonly)  NSString * _Nonnull appId;

/**
 *  String that identifies your App Key.
 */
@property (copy, nonatomic, readonly) NSString * _Nonnull appKey;

/**
 *  String that identifies the Monkey domain.
 */
@property (copy, nonatomic, readonly) NSString * _Nonnull domain;

/**
 *  String that identifies the Monkey port.
 */
@property (copy, nonatomic, readonly) NSString * _Nonnull port;

/**
 *  String that identifies the Monkey domain.
 */
@property (copy, nonatomic, readonly) NSArray * _Nonnull mediaTypes;

/**
 *  Dictionary which holds session params:
 *  - monkeyId -> Monkey Id
 *  - user -> User metadata
 *  - lastTimestamp -> Timestamp of last sync of messages
 *  - expireSession -> Boolean that determines if this monkey id expires with time on server
 *  - debuggingMode -> Boolean that determines development and production environments
 *  - autoSync -> Boolean that determines if the sync of messages should be automatic everytime the socket connects.
 */
@property (copy, nonatomic, readonly) NSMutableDictionary * _Nonnull session;

/**
 *	Initializes the session with Monkey
 *
 *  @param appId			Monkey App's Id
 *  @param appKey			Monkey App's secret
 *  @param user				User metadata
 *  @param shouldExpire		Flag that determines if the newly created Monkey Id should expire
 *  @param isDebugging		Flag that determines if the app is in Development or Production
 *  @param autoSync			Flag that determines if it should request pending messages upon connection
 *  @param lastTimestamp	Optional timestamp value from which pending messages will be fetched
 *  @param success			Completion block when Monkey was initialized successfully
 *  @param failure			Completion block when Monkey failed to initialize
 *
 *	@discussion	This should be the first thing you call with the Monkey singleton
 *
 *	@warning	You need to get your App Id and App Key from the Monkey Admin
 */
-(void)initWithApp:(nonnull NSString *)appId
            secret:(nonnull NSString *)appKey
              user:(nullable NSDictionary *)user
              ignoredParams:(nullable NSArray<NSString *> *)params
     expireSession:(BOOL)shouldExpire
         debugging:(BOOL)isDebugging
          autoSync:(BOOL)autoSync
     lastTimestamp:(nullable NSNumber*)lastTimestamp
           success:(nullable void (^)(NSDictionary * _Nonnull session))success
           failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Get session's Monkey Id
 *
 *	@return The Monkey Id for the current session.
 */
-(nullable NSString *)monkeyId;

/**
 *  Get session's User object
 *
 *  @return The user object with which Monkey was initialized.
 */
-(nullable NSDictionary *)user;

/**
 *  Close session's User object
 *
 *  @return boolean value of result.
 */
-(void)close;

/**
 *  Request pending messages
 */
-(void)getPendingMessages;

/**
 *  Request pending messages and request groups to which this monkey id belongs
 */
-(void)getPendingMessagesWithGroups;

/**
 *	Send open notification to a conversation
 */
-(void)openConversation:(nonnull NSString *)conversationId;

/**
 *  Send a text to a user
 *  
 *  @param  text			Plain text to send
 *  @param  shouldEncrypt	Flag that determines if the message should be encrypted
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *  @param  params			Optional params determined by the developer
 *  @param  push			Optional push that goes with the message
 *
 *	@return The message generated by Monkey
 *
 *	@discussion The push type is expected to be either `NSString` or `NSDictionary`
 */
-(nonnull MOKMessage *)sendText:(nonnull NSString *)text
                      encrypted:(BOOL)shouldEncrypt
                             to:(nonnull NSString *)conversationId
                         params:(nullable NSDictionary *)optionalParams
                           push:(nullable id)optionalPush;


/**
 *  Send a encrypted text to a user or group
 *
 *  @param  text			Plain text to send
 *  @param  shouldEncrypt	Flag that determines if the message should be encrypted
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *  @param  optionalParams	Optional params determined by the developer
 *  @param  optionalPush	Optional push that goes with the message
 *
 *	@return The message generated by Monkey
 *
 *	@discussion The push type is expected to be either `NSString` or `NSDictionary`
 */
-(nonnull MOKMessage *)sendEncryptedText:(nonnull NSString *)text
                                      to:(nonnull NSString *)conversationId
                                  params:(nullable NSDictionary *)optionalParams
                                    push:(nullable id)optionalPush;

/**
 *  Send a plain text to a user or group
 *
 *  @param  text			Plain text to send
 *  @param  shouldEncrypt	Flag that determines if the message should be encrypted
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *  @param  optionalParams	Optional params determined by the developer
 *  @param  optionalPush	Optional push that goes with the message
 *
 *	@return The message generated by Monkey
 *
 *	@discussion The push type is expected to be either `NSString` or `NSDictionary`
 */
-(nonnull MOKMessage *)sendText:(nonnull NSString *)text
                             to:(nonnull NSString *)conversationId
                         params:(nullable NSDictionary *)optionalParams
                           push:(nullable id)optionalPush;

/**
 *  Send a notification to a user
 *
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *	@param	optionalParams	Optional params determined by the developer
 *	@param	optionalPush	Optional push that goes with the notification
 *
 *	@return The message generated by Monkey
 *
 *	@discussion The push type is expected to be either `NSString` or `NSDictionary`.
 */
-(nonnull MOKMessage *)sendNotificationTo:(nonnull NSString *)conversationId
                                   params:(nullable NSDictionary *)optionalParams
                                     push:(nullable NSString *)optionalPush;

/**
 *  Send a temporal notification to a user
 *
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *	@param	optionalParams	Optional params determined by the developer
 *	@param	optionalPush	Optional push that goes with the notification
 *
 *	@return The message generated by Monkey
 *
 *	@discussion The push type is expected to be either `NSString` or `NSDictionary`
 *
 *	@warning This type of notification only works for messages received in real time.
 *	If the recipient is not online, he will never get this message.
 */
-(nonnull MOKMessage *)sendTemporalNotificationTo:(nonnull NSString *)conversationId
                                           params:(nullable NSDictionary *)optionalParams
                                             push:(nullable NSString *)optionalPush;

/**
 *  Delete a message from the server and notify conversation members
 *
 *	@param	messageId		The Id of the message to be deleted
 *	@param	conversationId	Conversation to be notified, it can be a Monkey Id or a Group Id
 *
 *	@discussion	Deleted messages won't arrive when requesting pending messages
 */
-(void)deleteMessage:(nonnull NSString *)messageId
              notify:(nonnull NSString *)conversationId;

/**
 *  Send a file from memory
 *
 *	@param	data			The data of the file
 *	@param	type			File type
 *	@param	filename		name of the file
 *  @param 	shouldEncrypt	Flag that determines if the file should be encrypted
 *	@param	shouldCompress	Flag that determines if the file should be compressed using GZIP
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *	@param	optionalParams	Optional params determined by the developer
 *	@param	optionalPush	Optional push that goes with the file message
 *  @param	success			Completion block when the file was sent successfully
 *  @param	failure			Completion block when the file failed to send
 *
 *	@return The message generated by Monkey which represents the file
 *
 *	@see MOKFileType
 */
-(nonnull MOKMessage *)sendFile:(nonnull NSData *)data
                           type:(MOKFileType)type
                       filename:(nonnull NSString *)filename
                      encrypted:(BOOL)shouldEncrypt
                     compressed:(BOOL)shouldCompress
                             to:(nonnull NSString *)conversationId
                         params:(nullable NSDictionary *)optionalParams
                           push:(nullable id)optionalPush
                        success:(nullable void (^)(MOKMessage * _Nonnull message))success
                        failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Send a file from a local path
 *
 *	@param	filePath		The path to your file
 *	@param	type			File type
 *	@param	filename		name of the file
 *  @param 	shouldEncrypt	Flag that determines if the file should be encrypted
 *	@param	shouldCompress	Flag that determines if the file should be compressed using GZIP
 *	@param	conversationId	Conversation Id, it can be a Monkey Id or a Group Id
 *	@param	optionalParams	Optional params determined by the developer
 *	@param	optionalPush	Optional push that goes with the file message
 *  @param	success			Completion block when the file was sent successfully
 *  @param	failure			Completion block when the file failed to send
 *
 *	@return The message generated by Monkey which represents the file
 *
 *	@see MOKFileType
 */
-(nullable MOKMessage *)sendFilePath:(nonnull NSString *)filePath
                                type:(MOKFileType)type
                            filename:(nonnull NSString *)filename
                           encrypted:(BOOL)shouldEncrypt
                          compressed:(BOOL)shouldCompress
                                  to:(nonnull NSString *)conversationId
                              params:(nullable NSDictionary *)optionalParams
                                push:(nullable id)optionalPush
                             success:(nullable void (^)(MOKMessage * _Nonnull message))success
                             failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Download file to specified folder.
 *
 *	@param 	message				Message that represents the file
 *	@param	fileDestination		Path where the file will be stored
 *  @param	success				Completion block when the file was downloaded successfully
 *  @param	failure				Completion block when the file failed to download
 *
 *	@discussion	The file will be decrypted and decompressed before being stored
 */
-(void)downloadFileMessage:(nonnull MOKMessage *)message
           fileDestination:(nonnull NSString *)fileDestination
                   success:(nullable void (^)(NSData * _Nonnull data))success
                   failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Get conversations for my Monkey Id.
 *
 *	@param	timestamp	Timestamp from which to request the next badge of conversations
 *	@param	qty			Quantity of conversations to fetch
 *  @param	success		Completion block when the conversations were fetched successfully
 *  @param	failure		Completion block when the conversations failed to fetch
 *
 *	@discussion	A conversation can be a group or between 2 Monkey Ids
 */
-(void)getConversationsSince:(double)timestamp
                    quantity:(int)qty
                     success:(nullable void (^)(NSArray<MOKConversation *> * _Nonnull conversations))success
                     failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 */
/**
 *	Get messages of a conversation
 *
 *  @param conversationId Id of the conversation
 *  @param timestamp      Timestamp from which to request the next badge of messages
 *  @param qty            Quantity of messages to fetch
 *  @param success        Completion block when the messages were fetched successfully
 *  @param failure        Completion block when the messages failed to fetch
 */

-(void)getConversationMessages:(nonnull NSString *)conversationId
                         since:(NSInteger)timestamp
                      quantity:(int)qty
                       success:(nullable void (^)(NSArray<MOKMessage *> * _Nonnull messages))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Create group with me and members
 *
 *  @param	optionalId   	Optional Id for the group
 *  @param	members      	Array of Monkey Ids
 *  @param	info         	Metadata defined by the developer
 *	@param	optionalPush	Optional push that goes with the file message
 *  @param	success       	Completion block when the group was created successfully
 *  @param	failure       	Completion block when the group failed to be created
 *
 *	@discussion	asdf
 */
-(void)createGroup:(nullable NSString *)optionalId
           members:(nonnull NSArray *)members
              info:(nullable NSMutableDictionary *)info
              push:(nullable id)optionalPush
           success:(nullable void (^)(NSDictionary * _Nonnull data))success
           failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Add member to existing group
 *
 *  @param newMonkeyId           Monkey Id of the new member
 *  @param groupId               Id of the group
 *  @param optionalPushNewMember Push for the new member
 *  @param optionalPushMembers   Push for existing members
 *  @param success               Completion block when the member was added successfully
 *  @param failure               Completion block when the request failed
 */
-(void)addMember:(nonnull NSString *)newMonkeyId
           group:(nonnull NSString *)groupId
   pushNewMember:(nullable id)optionalPushNewMember
     pushMembers:(nullable id)optionalPushMembers
         success:(nullable void (^)(NSDictionary * _Nonnull data))success
         failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Remove member from existing group
 *
 *  @param monkeyId Monkey Id of the member to be deleted
 *  @param groupId  Id of the group
 *  @param success  Completion block when the member was removed successfully
 *  @param failure  Completion block when the request failed
 */
-(void)removeMember:(nonnull NSString *)monkeyId
              group:(nonnull NSString *)groupId
            success:(nullable void (^)(NSDictionary * _Nonnull data))success
            failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Delete conversation from my history of conversations
 *
 *  @param conversationId	Id of the conversation
 *  @param success			Completion block when the conversation was deleted successfully
 *  @param failure			Completion block when the request failed
 */
-(void)deleteConversation:(nonnull NSString *)conversationId
                  success:(nullable void (^)(NSDictionary * _Nonnull data))success
                  failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *  Get info of user or group, whichever is the case
 *
 *  @param conversationId	Id of the group or user to request info
 *  @param success			Completion block when the conversation was deleted successfully
 *  @param failure  		Completion block when the request failed
 */
-(void)getInfo:(nonnull NSString *)conversationId
       success:(nullable void (^)(NSDictionary * _Nonnull data))success
       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *	Get info of a list of monkey ids
 *
 *  @param idList	Array os Monkey Ids
 *  @param success	Completion block when the metadata was fetched successfully
 *  @param failure	Completion block when the request failed
 */
-(void)getInfoByIds:(nonnull NSArray *)idList
           success:(nullable void (^)(NSArray <MOKUser *> * _Nonnull data))success
           failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error))failure;

/**
 *	Utils functions
 */

/**
 *	Check if the message is outgoing
 */
-(BOOL)isMessageOutgoing:(nonnull MOKMessage *)message;

@end

///--------------------
/// @name Notifications
///--------------------

/**
 *	Posted when the socket connection status is changed.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeySocketStatusChangeNotification;

/**
 *	Posted when a message arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyMessageNotification;

/**
 *	Posted when a notification arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyNotificationNotification;

/**
 *	Posted when an acknowledgement to a message I sent, arrives throught the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyAcknowledgeNotification;

/**
 *	Posted when a group creation notification arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyGroupCreateNotification;

/**
 *	Posted when a group member removal notification arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyGroupRemoveNotification;

/**
 *	Posted when a group add member notification arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyGroupAddNotification;

/**
 *	Posted when the list of group (ids) that I belong to, arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyGroupListNotification;

/**
 *	Posted when an open notification arrives through the socket.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyOpenNotification;

/**
 *	Posted as a response to an open I did, or when the status of a relevant conversation changes
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyConversationStatusNotification;

/**
 *	Posted when someone closes a conversation with me.
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyCloseNotification;

/**
 *	Posted when the app should store the message
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyMessageStoreNotification;

/**
 *	Posted when the app should delete the message
 */
FOUNDATION_EXPORT NSString * __nonnull const MonkeyMessageDeleteNotification;
