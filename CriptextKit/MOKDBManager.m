//
//  MOKDBManager.m
//  MonkeyKit
//
//  Created by Gianni Carlo on 3/24/15.
//  Copyright (c) 2015 Criptext. All rights reserved.
//

#import "MOKDBManager.h"
#import "MOKMessage.h"
#import "MOKDBMessage.h"
#import "MOKDBSession.h"
#import "MOKUserDictionary.h"
#import "MOKJSON.h"
#import <Realm/Realm.h>

@interface MOKDBManager ()
@property (nonatomic, strong) MOKSBJsonWriter *jsonWriter;
@property (nonatomic, strong) MOKSBJsonParser *jsonParser;
@end

@implementation MOKDBManager
#pragma mark - initialization
+ (instancetype)sharedInstance
{
    static MOKDBManager *sharedInstance;
    
    if (!sharedInstance) {
        sharedInstance = [[self alloc] initPrivate];
    }
    
    return sharedInstance;
}

- (instancetype)init
{
    @throw [NSException exceptionWithName:@"Singleton"
                                   reason:@"Use +[MOKDBManager sharedInstance]"
                                 userInfo:nil];
    return nil;
}

- (instancetype)initPrivate
{
    self = [super init];
    if (self) {
        //init properties
        self.jsonWriter = [MOKSBJsonWriter new];
        self.jsonParser = [MOKSBJsonParser new];
        [self setDatabaseSchema];
        [RLMRealm setEncryptionKey:[self getKey] forRealmsAtPath:[self getCustomRealm]];
    }
    return self;
}

- (NSString *)getCustomRealm{
    NSString *documentsDirectory = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    documentsDirectory = [documentsDirectory stringByAppendingPathComponent:@"private_MOKdb"];
    NSString *customRealmPath = [documentsDirectory stringByAppendingString:@".realm"];
    return customRealmPath;
}

- (void)setDatabaseSchema{
    [RLMRealm setSchemaVersion:5 forRealmAtPath:[self getCustomRealm] withMigrationBlock:^(RLMMigration *migration, NSUInteger oldSchemaVersion) {
        if (oldSchemaVersion < 1) {
            [migration enumerateObjects:MOKDBMessage.className block:^(RLMObject *oldObject, RLMObject *newObject) {
                newObject[@"timestampCreated"] = oldObject[@"timestamp"];
                newObject[@"timestampOrder"] = oldObject[@"timestamp"];
            }];
        }
        if (oldSchemaVersion < 2) {
            [migration enumerateObjects:MOKDBMessage.className block:^(RLMObject *oldObject, RLMObject *newObject) {
                newObject[@"protocolCommand"] = [NSNumber numberWithInt:200];
                if ((int)oldObject[@"type"] == 54 || (int)oldObject[@"type"] == 55) {
                    newObject[@"protocolType"] = [NSNumber numberWithInt:2];
                }else{
                    newObject[@"protocolType"] = [NSNumber numberWithInt:1];
                }
            }];
        }
    }];
}

- (NSData *)getKey {
    // Identifier for our keychain entry - should be unique for your application
    static const uint8_t kKeychainIdentifier[] = "com.criptext.monkeykit";
    NSData *tag = [[NSData alloc] initWithBytesNoCopy:(void *)kKeychainIdentifier
                                               length:sizeof(kKeychainIdentifier)
                                         freeWhenDone:NO];
    
    // First check in the keychain for an existing key
    NSDictionary *query = @{(__bridge id)kSecClass: (__bridge id)kSecClassKey,
                            (__bridge id)kSecAttrApplicationTag: tag,
                            (__bridge id)kSecAttrKeySizeInBits: @512,
                            (__bridge id)kSecReturnData: @YES};
    
    CFTypeRef dataRef = NULL;
    OSStatus status = SecItemCopyMatching((__bridge CFDictionaryRef)query, &dataRef);
    if (status == errSecSuccess) {
        return (__bridge NSData *)dataRef;
    }
    
    // No pre-existing key from this application, so generate a new one
    uint8_t buffer[64];
    SecRandomCopyBytes(kSecRandomDefault, 64, buffer);
    NSData *keyData = [[NSData alloc] initWithBytes:buffer length:sizeof(buffer)];
    
    // Store the key in the keychain
    query = @{(__bridge id)kSecClass: (__bridge id)kSecClassKey,
              (__bridge id)kSecAttrApplicationTag: tag,
              (__bridge id)kSecAttrKeySizeInBits: @512,
              (__bridge id)kSecValueData: keyData};
    
    status = SecItemAdd((__bridge CFDictionaryRef)query, NULL);
    NSAssert(status == errSecSuccess, @"Failed to insert new key in the keychain");
    
    return keyData;
}
#pragma mark - Messages
- (void)storeMessage:(MOKMessage *)msg{
    
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    NSLog(@"MONKEY - MONKEY - params saved: %@", msg.params);
    
    [realm beginWriteTransaction];
    NSDictionary *object = @{
                             @"messageId": @(msg.messageId),
                             @"oldmessageId": @(msg.oldMessageId),
                             @"userIdFrom": msg.userIdFrom,
                             @"userIdTo": msg.userIdTo,
                             @"protocolCommand": @(msg.protocolCommand),
                             @"protocolType": @(msg.protocolType),
                             @"timestampCreated": @(msg.timestampCreated),
                             @"timestampOrder": @(msg.timestampOrder),
                             @"messageText": msg.messageText,
                             @"iv": msg.iv ? msg.iv : @"0",
                             @"readByUser": @(msg.readByUser),
                             @"param": [self.jsonWriter stringWithObject:msg.params]
                             };
    
    
    [MOKDBMessage createOrUpdateInRealm:realm withObject:object];
    [realm commitWriteTransaction];
}
- (BOOL)existMessage:(MOKMessageId)messageId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    return [MOKDBMessage objectInRealm:realm forPrimaryKey:[NSNumber numberWithLongLong:messageId]] != nil;
}
- (MOKMessage *)getMessageById:(MOKMessageId )messageId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBMessage *msg = [MOKDBMessage objectInRealm:realm forPrimaryKey:[NSNumber numberWithLongLong:messageId]];
    
    if (msg !=nil) {
        MOKMessage *mensaje = [[MOKMessage alloc]init];
        mensaje.messageText = msg.messageText;
        mensaje.protocolCommand = (int)msg.protocolCommand;
        mensaje.protocolType = (int)msg.protocolType;
        mensaje.timestampCreated = msg.timestampCreated;
        mensaje.timestampOrder = msg.timestampOrder;
        mensaje.messageId = msg.messageId;
        mensaje.userIdFrom = msg.userIdFrom;
        mensaje.userIdTo = msg.userIdTo;
        mensaje.readByUser = msg.readByUser;
        mensaje.oldMessageId = msg.oldmessageId;
        mensaje.params = [self.jsonParser objectWithString:msg.param];
        return mensaje;
    }else{
        return nil;
    }
}
- (void)deleteMessageSent:(MOKMessage *)msg{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBMessage *mensaje = [MOKDBMessage objectInRealm:realm forPrimaryKey:[NSNumber numberWithLongLong:msg.oldMessageId]];
    if (!mensaje) {
        return;
    }
    [realm beginWriteTransaction];
    
    [realm deleteObject:mensaje];
    
    [realm commitWriteTransaction];
    
}
- (MOKMessage *)getOldestMessageNotSent{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    for (MOKDBMessage *msg in [MOKDBMessage allObjectsInRealm:realm]) {
        if (msg.messageId<0) {
            MOKMessage *message = [[MOKMessage alloc]init];
            message.messageId = msg.messageId;
            message.userIdTo = msg.userIdTo;
            message.userIdFrom = msg.userIdFrom;
            message.protocolCommand = (int)msg.protocolCommand;
            message.protocolType = (int)msg.protocolType;
            message.timestampCreated = msg.timestampCreated;
            message.timestampOrder = msg.timestampOrder;
            message.messageText = msg.messageText;
            message.readByUser = msg.readByUser;
            message.params = [self.jsonParser objectWithString:msg.param];
            if (message.params == nil) {
                message.params = [@{@"eph":@"0"} mutableCopy];
            }
            message.oldMessageId = msg.oldmessageId;
            return message;
        }
    }
    return nil;
}

- (MOKDBSession *)checkSession:(RLMRealm *)realm{
    RLMResults *sessionresults = [MOKDBSession allObjectsInRealm:realm];
    MOKDBSession *session;
    
    if(sessionresults.count == 0){
        session = [[MOKDBSession alloc]init];
    }else{
        session = sessionresults.firstObject;
    }
    return session;
}

//- (MOKDBSession *)loadSessionFromDB{
//    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
//    MOKDBSession *session = [self checkSession:realm];
//    
//    return session;
//}
//- (void)storeSession{
//    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
//    MOKDBSession *session = [self checkSession:realm];
//    
//    
//    [MOKDBSession createOrUpdateInRealm:realm withObject:session];
//}

- (void)storeSessionId:(NSString *)sessionId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    [realm beginWriteTransaction];
    session.sessionId = sessionId;
    [MOKDBSession createOrUpdateInRealm:realm withObject:session];
    [realm commitWriteTransaction];
}
- (NSString *)loadSessionId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    return session.sessionId;

}
- (void)storeAppId:(NSString *)appId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    [realm beginWriteTransaction];
    session.appId = appId;
    [MOKDBSession createOrUpdateInRealm:realm withObject:session];
    [realm commitWriteTransaction];
}
- (NSString *)loadAppId{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    return session.appId;
}
- (void)storeAppKey:(NSString *)appKey{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    [realm beginWriteTransaction];
    session.appKey = appKey;
    [MOKDBSession createOrUpdateInRealm:realm withObject:session];
    [realm commitWriteTransaction];
}
- (NSString *)loadAppKey{
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    return session.appKey;
}
- (void)storeUser:(MOKUserDictionary *)user{
    NSLog(@"MONKEY - store user");
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    
    [realm beginWriteTransaction];
    session.user = [self.jsonWriter stringWithObject:user];
    [MOKDBSession createOrUpdateInRealm:realm withObject:session];
    [realm commitWriteTransaction];
}
- (MOKUserDictionary *)loadUser{
    NSLog(@"MONKEY - loading user");
    RLMRealm *realm = [RLMRealm realmWithPath:[self getCustomRealm]];
    MOKDBSession *session = [self checkSession:realm];
    MOKUserDictionary *user = [self.jsonParser objectWithString:session.user];
    return user;
}
@end