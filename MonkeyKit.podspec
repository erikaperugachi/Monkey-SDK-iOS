#
# Be sure to run `pod lib lint MonkeyKit.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'MonkeyKit'
  s.version          = '1.0.7'
  s.summary          = 'iOS SDK to access Monkey secure channel'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
MonkeyKit makes encryption easier for developers.
                       DESC

  s.homepage         = 'https://criptext.com'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'Apache 2.0', :file => 'LICENSE' }
  s.author           = { 'Criptext, Inc.' => 'gianni@criptext.com' }
  s.source           = { :git => 'https://github.com/Criptext/iOS-MonkeySDK.git', :tag => s.version.to_s }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '8.0'

  s.source_files = 'MonkeyKit/Classes/**/*'
  
  # s.resource_bundles = {
  #   'MonkeyKit' => ['MonkeyKit/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  
  s.dependency "AFNetworking"
  s.dependency 'UICKeyChainStore'
end
