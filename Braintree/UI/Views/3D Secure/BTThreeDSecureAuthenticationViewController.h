@import UIKit;

#import "BTThreeDSecureLookupResult.h"
#import "BTThreeDSecureErrors.h"

typedef NS_ENUM(NSInteger, BTThreeDSecureViewControllerCompletionStatus) {
    BTThreeDSecureViewControllerCompletionStatusFailure = 0,
    BTThreeDSecureViewControllerCompletionStatusSuccess,
};

@protocol BTThreeDSecureAuthenticationViewControllerDelegate;

///  A view controller that facilitates user authentication for 3D Secure
///
///  @warning In most cases, merchants should utilize BTThreeDSecure, rather than
///           initializing this view controller directly.
///
///  @see BTThreeDSecure
///
///  This view controller accepts a "lookup", which must be obtained via
///  -[BTClient lookupNonceForThreeDSecure:transactionAmount:success:failure:] and uses a web view
///  to present the issuing bank's login form to the user.
///
///  An initialized BTThreeDSecureViewController will challenge the user as soon as it is presented
///  and cannot be reused.
///
///  On success, the original payment method nonce is consumed, and you will receive a new payment
///  method nonce. Transactions created with this nonce will be 3D Secure.
///
///  Sometimes, this view controller will not be necessary to achieve the liabilty shift. In these cases,
///  lookup will have already consumed the original nonce and returned a new one.
@interface BTThreeDSecureAuthenticationViewController : UIViewController

///  Initializes a 3D Secure authentication view controller
///
///  @param lookup Contains metadata about the 3D Secure lookup
///
///  @return A view controller or nil when authentication is not possible and/or required.
- (instancetype)initWithLookup:(BTThreeDSecureLookupResult *)lookup NS_DESIGNATED_INITIALIZER;

///  The delegate is notified when the 3D Secure authentication flow completes
@property (nonatomic, weak) id<BTThreeDSecureAuthenticationViewControllerDelegate> delegate;

@end

@protocol BTThreeDSecureAuthenticationViewControllerDelegate <NSObject>

///  The delegate will receive this message after the user has successfully authenticated with 3D Secure
///
///  This nonce will point to a card that has a 3D Secure verification with successful authentication (Y or A) and successful signature verification (Y).
///
///  This implementation is responsible for receiving the 3D Secure payment method nonce, possibly transmitting
///  it to your server for server-side operations. Upon completion, you must call the completionBlock.
///
///  Do *not* dismiss the view controller in this method. See threeDSecureViewControllerDidFinish:.
///
///  @param viewController  The 3D Secure view controller
///  @param nonce           The new payment method nonce that should be used for creating a 3D Secure transaction
///  @param completionBlock A required
- (void)threeDSecureViewController:(BTThreeDSecureAuthenticationViewController *)viewController
              didAuthenticateNonce:(NSString *)nonce
                        completion:(void (^)(BTThreeDSecureViewControllerCompletionStatus success))completionBlock;

///  The delegate will receive this message when 3D Secure authentication fails
///
///  This can occur due to a system error, lack of issuer participation or failed user authentication.

///  Do *not* dismiss the view controller in this method. See threeDSecureViewControllerDidFinish:.
///
///  @param viewController  The 3D Secure view controller
///  @param error           The error that caused 3D Secure to fail
- (void)threeDSecureViewController:(BTThreeDSecureAuthenticationViewController *)viewController
                  didFailWithError:(NSError *)error;

///  The delegate will receive this message upon completion of the 3D Secure flow, possibly including async work
///  that happens in your implementation of threeDSecureViewController:didAuthenticateNonce:completion:
///
/// This method will be called in both success and failure cases.
///
///  You should dismiss the provided view controller in your implementation.
///
///  @param viewController The 3D Secure view controller
- (void)threeDSecureViewControllerDidFinish:(BTThreeDSecureAuthenticationViewController *)viewController;

@end
