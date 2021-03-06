// Thanks https://github.com/JulianAtGitHub/OpenGL-tutorial-for-Cocoa-master

#import <Cocoa/Cocoa.h>
@protocol CashewOpenGLViewDelegate;

@interface CashewOpenGLView : NSOpenGLView {
    NSTimer* _timer;
    NSTrackingArea* trackingArea;
}

@property(nonatomic, strong) id<CashewOpenGLViewDelegate> delegate;
- (void)visit:(NSTimer*)theTimer;

@end

@protocol CashewOpenGLViewDelegate<NSObject>

@required
- (BOOL)prepareRenderData;
- (void)update:(NSTimeInterval)timeInterval;
- (void)render;
- (void)reshapeRect:(NSRect)rect backing:(NSRect)backing;
@end

#define Set_OpenGLViewDelegate(classname)                                  \
    classname* delegate = [[classname alloc] init];                        \
    self.delegate = delegate;                                              \
    if ([self.delegate respondsToSelector:@selector(prepareRenderData)]) { \
        [self.delegate prepareRenderData];                                 \
    }                                                                      \
    _timer = [NSTimer timerWithTimeInterval:(1.0 / 60.0)                   \
                                     target:self                           \
                                   selector:@selector(visit:)              \
                                   userInfo:nil                            \
                                    repeats:YES];                          \
    [[NSRunLoop currentRunLoop] addTimer:_timer forMode:NSRunLoopCommonModes];
