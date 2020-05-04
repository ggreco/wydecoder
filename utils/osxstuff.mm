#include "logger.h"
#import <Cocoa/Cocoa.h>
#import <IOKit/pwr_mgt/IOPMLib.h>

@interface DeepLink : NSObject
@end

static void(*uri_function)(const char*);
static __strong DeepLink *_instance;

@implementation DeepLink

+(DeepLink*)instance
{ return _instance; }

-(instancetype)init
{
    self = [super init];
    return self;
}

- (void)handleURLEvent:(NSAppleEventDescriptor*)event withReplyEvent:(NSAppleEventDescriptor*)replyEvent
{
    NSString* url = [[event paramDescriptorForKeyword:keyDirectObject] stringValue];
    ILOG << "Received URL";
    uri_function([url UTF8String]);
}

+(void)load
{
    _instance = [DeepLink new];
    NSAppleEventManager *appleEventManager = [NSAppleEventManager sharedAppleEventManager];
    [appleEventManager setEventHandler:_instance andSelector:@selector(handleURLEvent:withReplyEvent:) forEventClass:kInternetEventClass andEventID:kAEGetURL];
}

@end

void register_uri_handler(void(*fnc)(const char*))
{
    uri_function = fnc;
    [DeepLink load];
}

void unregister_uri_handler()
{
    if (_instance) {
        NSAppleEventManager *appleEventManager = [NSAppleEventManager sharedAppleEventManager];
        [appleEventManager removeEventHandlerForEventClass:kInternetEventClass andEventID:kAEGetURL];
    }
}

const char *get_lang_id()
{
    NSString * language = [[NSLocale preferredLanguages] objectAtIndex:0];

    return [language UTF8String];
}

void flush_events() {
    int idx = 0;
    ILOG << "Flushing events...";
     for ( ; ; ) {
        NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES ];
        if ( event == nil ) {
            ILOG << idx << " events flushed!";
            break;
        }
        ++idx;

        // Pass events down to SDLApplication to be handled in sendEvent:
        [NSApp sendEvent:event];
     }
}

const char *get_temporary_dir()
{
    NSString *path;
    NSString *bundleName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"];

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    if ([paths count]) {
        path = [[paths objectAtIndex:0] stringByAppendingPathComponent:bundleName];
    } else {
        path = NSTemporaryDirectory();
        path = [path stringByAppendingPathComponent:bundleName];
    }
    return [path UTF8String];
}

std::string
get_os()
{
    NSOperatingSystemVersion v = [[NSProcessInfo processInfo] operatingSystemVersion];
    std::ostringstream os;
    os << "MacOS " << v.majorVersion << '.' << v.minorVersion << '.' << v.patchVersion;
    return os.str();
}

long long block_sleep()
{
    CFStringRef reasonForActivity = CFSTR("Wyscout Uploader activity");

    IOPMAssertionID assertionID;
    IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoIdleSleep,
                                        kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    if (success == kIOReturnSuccess) {
        ILOG << "Sleep prevention started";
        return assertionID;
    }
    else
        return -1;
}

void restore_sleep(long long assertionID)
{
    if (assertionID != -1)
        IOPMAssertionRelease(assertionID);
}

#include "SDL.h"
#include "SDL_syswm.h"

void osx_always_on_top(SDL_Window *window)
{
    struct SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info)) {
        [info.info.cocoa.window setLevel:NSFloatingWindowLevel];
    } else
        ELOG << "Unable to get window informations for:" << (void*)window << ": " << SDL_GetError();
}