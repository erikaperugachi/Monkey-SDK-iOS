//
//  MOKDBManager.h
//  MonkeyKit
//
//  Created by Gianni Carlo on 3/24/15.
//  Copyright (c) 2015 Criptext. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MOKutils.h"
@class MOKMessage;
@class MOKUserDictionary;
@class MOKDBSession;
@interface MOKDBManager : NSObject
+ (MOKDBManager*) sharedInstance;

//ongoing msgs
- (void)storeMessage:(MOKMessage *)msg;
- (BOOL)existMessage:(MOKMessageId)messageId;
- (MOKMessage *)getMessageById:(MOKMessageId )messageId;
- (void)deleteMessageSent:(MOKMessage *)msg;
- (MOKMessage *)getOldestMessageNotSent;

//session
- (MOKDBSession *)loadSessionFromDB;
- (void)storeSessionFromDB;

- (void)storeSessionId:(NSString *)sessionId;
- (NSString *)loadSessionId;
- (void)storeLastMessageId:(NSString *)messageId;
- (NSString *)loadLastMessageId;
- (void)storeAppId:(NSString *)appId;
- (NSString *)loadAppId;
- (void)storeAppKey:(NSString *)appKey;
- (NSString *)loadAppKey;
- (void)storeUser:(MOKUserDictionary *)user;
- (MOKUserDictionary *)loadUser;
@end