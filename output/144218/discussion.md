# crash on "<invalid>: unknown current parser token" and a NULL-filled file (that's not NULL-filled)

**Author:** midnightveil
**URL:** https://github.com/llvm/llvm-project/issues/144218
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, needs-reduction
**Closed Date:** 2025-06-16T23:01:35Z

## Body

```cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin/clang++ @/tmp/cc-params.CRTKnJ
1.      <invalid>: unknown current parser token
  #0 0x00007f2b046dd2de llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedd2de)
  #1 0x00007f2b046db04c llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedb04c)
  #2 0x00007f2b045cd498 CrashRecoverySignalHandler(int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xdcd498)
  #3 0x00007f2b03247620 __restore_rt (/nix/store/wn7v2vhyyyi6clcyn0s9ixvl7d4d87ic-glibc-2.40-36/lib/libc.so.6+0x40620)
  #4 0x00007f2b0cf7a50c clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb7a50c)
  #5 0x00007f2b0cf8178f clang::Lexer::LexTokenInternal(clang::Token&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb8178f)
  #6 0x00007f2b0cfc62a5 clang::Preprocessor::SkipExcludedConditionalBlock(clang::SourceLocation, clang::SourceLocation, bool, bool, clang::SourceLocation) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc62a5)
  #7 0x00007f2b0cfc7dfa clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc7dfa)
  #8 0x00007f2b0cfcd323 clang::Preprocessor::HandleDirective(clang::Token&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbcd323)
  #9 0x00007f2b0cf831ce clang::Lexer::LexTokenInternal(clang::Token&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb831ce)
 #10 0x00007f2b0d0036b4 clang::Preprocessor::Lex(clang::Token&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc036b4)
 #11 0x00007f2b0d02356a clang::Parser::ConsumeAnyToken(bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc2356a)
 #12 0x00007f2b0d1158ba clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd158ba)
// continues

```

preprocessed source and run-script.
[source.zip](https://github.com/user-attachments/files/20737535/source.zip)

Occurred while making modifications https://git.lix.systems/lix-project/lix. I'm not quite sure what happened here, I think somehow the in memory representation became a whole bunch of zeroes and clang got extremely confused. It went away after rebuilding. Suspect this might have something to do with precompiled headers? (gut feeling)


<details><summary>Entire output</summary>

```
   62 |     JSON toJSON(Store & store) const;
      |                      ^
../lix/libcmd/cmd-profiles.hh:62:23: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                       ^
../lix/libcmd/cmd-profiles.hh:62:24: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                        ^
../lix/libcmd/cmd-profiles.hh:62:28: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                            ^
../lix/libcmd/cmd-profiles.hh:62:29: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                             ^
../lix/libcmd/cmd-profiles.hh:62:30: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                              ^
../lix/libcmd/cmd-profiles.hh:62:31: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                               ^
../lix/libcmd/cmd-profiles.hh:62:32: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                                ^
../lix/libcmd/cmd-profiles.hh:62:36: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                                    ^
../lix/libcmd/cmd-profiles.hh:62:37: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                                     ^
../lix/libcmd/cmd-profiles.hh:62:38: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store & store) const;
      |                                      ^
../lix/libcmd/cmd-profiles.hh:63:1: warning: null character ignored [-Wnull-character]
   63 | 
      | ^
../lix/libcmd/cmd-profiles.hh:64:1: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      | ^
../lix/libcmd/cmd-profiles.hh:64:5: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |     ^
../lix/libcmd/cmd-profiles.hh:64:6: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |      ^
../lix/libcmd/cmd-profiles.hh:64:7: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |       ^
../lix/libcmd/cmd-profiles.hh:64:8: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |        ^
../lix/libcmd/cmd-profiles.hh:64:9: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |         ^
../lix/libcmd/cmd-profiles.hh:64:11: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |           ^
../lix/libcmd/cmd-profiles.hh:64:12: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |            ^
../lix/libcmd/cmd-profiles.hh:64:13: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |             ^
../lix/libcmd/cmd-profiles.hh:64:14: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |              ^
../lix/libcmd/cmd-profiles.hh:64:15: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |               ^
../lix/libcmd/cmd-profiles.hh:64:16: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                ^
../lix/libcmd/cmd-profiles.hh:64:17: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                 ^
../lix/libcmd/cmd-profiles.hh:64:19: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                   ^
../lix/libcmd/cmd-profiles.hh:64:20: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                    ^
../lix/libcmd/cmd-profiles.hh:64:21: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                     ^
../lix/libcmd/cmd-profiles.hh:64:22: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                      ^
../lix/libcmd/cmd-profiles.hh:64:23: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                       ^
../lix/libcmd/cmd-profiles.hh:64:24: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                        ^
../lix/libcmd/cmd-profiles.hh:64:25: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                         ^
../lix/libcmd/cmd-profiles.hh:64:27: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                           ^
../lix/libcmd/cmd-profiles.hh:64:28: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                            ^
../lix/libcmd/cmd-profiles.hh:64:29: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                             ^
../lix/libcmd/cmd-profiles.hh:64:30: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                              ^
../lix/libcmd/cmd-profiles.hh:64:31: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                               ^
../lix/libcmd/cmd-profiles.hh:64:32: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                ^
../lix/libcmd/cmd-profiles.hh:64:33: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                 ^
../lix/libcmd/cmd-profiles.hh:64:35: error: source file is not valid UTF-8
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                   ^
../lix/libcmd/cmd-profiles.hh:64:39: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                       ^
../lix/libcmd/cmd-profiles.hh:64:40: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                        ^
../lix/libcmd/cmd-profiles.hh:64:41: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                         ^
../lix/libcmd/cmd-profiles.hh:64:45: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                             ^
../lix/libcmd/cmd-profiles.hh:64:46: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                              ^
../lix/libcmd/cmd-profiles.hh:64:47: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                               ^
../lix/libcmd/cmd-profiles.hh:64:48: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                ^
../lix/libcmd/cmd-profiles.hh:64:49: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                 ^
../lix/libcmd/cmd-profiles.hh:64:53: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                     ^
../lix/libcmd/cmd-profiles.hh:64:54: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                      ^
../lix/libcmd/cmd-profiles.hh:64:55: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                       ^
../lix/libcmd/cmd-profiles.hh:64:56: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                        ^
../lix/libcmd/cmd-profiles.hh:64:57: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise<Result<StorePath>> build(ref<Store> store);
      |                                                         ^
../lix/libcmd/cmd-profiles.hh:65:1: warning: null character ignored [-Wnull-character]
   65 | 
      | ^
../lix/libcmd/cmd-profiles.hh:66:1: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      | ^
../lix/libcmd/cmd-profiles.hh:66:2: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |  ^
../lix/libcmd/cmd-profiles.hh:66:3: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |   ^
../lix/libcmd/cmd-profiles.hh:66:4: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |    ^
../lix/libcmd/cmd-profiles.hh:66:6: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |      ^
../lix/libcmd/cmd-profiles.hh:66:7: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |       ^
../lix/libcmd/cmd-profiles.hh:66:8: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |        ^
../lix/libcmd/cmd-profiles.hh:66:9: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |         ^
../lix/libcmd/cmd-profiles.hh:66:10: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |          ^
../lix/libcmd/cmd-profiles.hh:66:11: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |           ^
../lix/libcmd/cmd-profiles.hh:66:12: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |            ^
../lix/libcmd/cmd-profiles.hh:66:14: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |              ^
../lix/libcmd/cmd-profiles.hh:66:15: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |               ^
../lix/libcmd/cmd-profiles.hh:66:16: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                ^
../lix/libcmd/cmd-profiles.hh:66:17: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                 ^
../lix/libcmd/cmd-profiles.hh:66:18: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                  ^
../lix/libcmd/cmd-profiles.hh:66:19: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                   ^
../lix/libcmd/cmd-profiles.hh:66:20: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                    ^
../lix/libcmd/cmd-profiles.hh:66:22: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                      ^
../lix/libcmd/cmd-profiles.hh:66:23: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                       ^
../lix/libcmd/cmd-profiles.hh:66:24: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                        ^
../lix/libcmd/cmd-profiles.hh:66:25: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                         ^
../lix/libcmd/cmd-profiles.hh:66:26: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                          ^
../lix/libcmd/cmd-profiles.hh:66:27: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                           ^
../lix/libcmd/cmd-profiles.hh:66:28: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                            ^
../lix/libcmd/cmd-profiles.hh:66:30: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                              ^
../lix/libcmd/cmd-profiles.hh:66:34: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                  ^
../lix/libcmd/cmd-profiles.hh:66:35: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                   ^
../lix/libcmd/cmd-profiles.hh:66:36: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                    ^
../lix/libcmd/cmd-profiles.hh:66:37: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                     ^
../lix/libcmd/cmd-profiles.hh:66:40: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                        ^
../lix/libcmd/cmd-profiles.hh:66:41: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                         ^
../lix/libcmd/cmd-profiles.hh:66:42: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                          ^
../lix/libcmd/cmd-profiles.hh:66:43: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                           ^
../lix/libcmd/cmd-profiles.hh:66:44: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                            ^
../lix/libcmd/cmd-profiles.hh:66:45: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                             ^
../lix/libcmd/cmd-profiles.hh:66:48: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                ^
../lix/libcmd/cmd-profiles.hh:66:49: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                 ^
../lix/libcmd/cmd-profiles.hh:66:50: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                  ^
../lix/libcmd/cmd-profiles.hh:66:51: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                   ^
../lix/libcmd/cmd-profiles.hh:66:52: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                    ^
../lix/libcmd/cmd-profiles.hh:66:53: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                     ^
../lix/libcmd/cmd-profiles.hh:66:56: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                        ^
../lix/libcmd/cmd-profiles.hh:66:57: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                         ^
../lix/libcmd/cmd-profiles.hh:66:58: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                          ^
../lix/libcmd/cmd-profiles.hh:66:59: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                           ^
../lix/libcmd/cmd-profiles.hh:66:60: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                            ^
../lix/libcmd/cmd-profiles.hh:66:61: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                             ^
../lix/libcmd/cmd-profiles.hh:66:63: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                               ^
../lix/libcmd/cmd-profiles.hh:66:64: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                ^
../lix/libcmd/cmd-profiles.hh:66:65: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                 ^
../lix/libcmd/cmd-profiles.hh:66:66: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                  ^
../lix/libcmd/cmd-profiles.hh:66:67: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                   ^
../lix/libcmd/cmd-profiles.hh:66:68: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                    ^
../lix/libcmd/cmd-profiles.hh:66:69: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                     ^
../lix/libcmd/cmd-profiles.hh:66:71: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                       ^
../lix/libcmd/cmd-profiles.hh:66:72: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                        ^
../lix/libcmd/cmd-profiles.hh:66:73: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                         ^
../lix/libcmd/cmd-profiles.hh:66:74: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                          ^
../lix/libcmd/cmd-profiles.hh:66:75: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                           ^
../lix/libcmd/cmd-profiles.hh:66:76: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                            ^
../lix/libcmd/cmd-profiles.hh:67:1: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      | ^
../lix/libcmd/cmd-profiles.hh:67:2: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |  ^
../lix/libcmd/cmd-profiles.hh:67:3: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |   ^
../lix/libcmd/cmd-profiles.hh:67:4: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |    ^
../lix/libcmd/cmd-profiles.hh:67:5: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |     ^
../lix/libcmd/cmd-profiles.hh:67:6: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |      ^
../lix/libcmd/cmd-profiles.hh:67:7: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |       ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin/clang++ @/tmp/cc-params.CRTKnJ
1.      <invalid>: unknown current parser token
  #0 0x00007f2b046dd2de llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedd2de)
  #1 0x00007f2b046db04c llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedb04c)
  #2 0x00007f2b045cd498 CrashRecoverySignalHandler(int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xdcd498)
  #3 0x00007f2b03247620 __restore_rt (/nix/store/wn7v2vhyyyi6clcyn0s9ixvl7d4d87ic-glibc-2.40-36/lib/libc.so.6+0x40620)
  #4 0x00007f2b0cf7a50c clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb7a50c)
  #5 0x00007f2b0cf8178f clang::Lexer::LexTokenInternal(clang::Token&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb8178f)
  #6 0x00007f2b0cfc62a5 clang::Preprocessor::SkipExcludedConditionalBlock(clang::SourceLocation, clang::SourceLocation, bool, bool, clang::SourceLocation) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc62a5)
  #7 0x00007f2b0cfc7dfa clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc7dfa)
  #8 0x00007f2b0cfcd323 clang::Preprocessor::HandleDirective(clang::Token&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbcd323)
  #9 0x00007f2b0cf831ce clang::Lexer::LexTokenInternal(clang::Token&, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb831ce)
 #10 0x00007f2b0d0036b4 clang::Preprocessor::Lex(clang::Token&) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc036b4)
 #11 0x00007f2b0d02356a clang::Parser::ConsumeAnyToken(bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc2356a)
 #12 0x00007f2b0d1158ba clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd158ba)
 #13 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #14 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #15 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #16 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #17 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #18 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #19 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 // repeated another 200 times
#252 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#253 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#254 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#255 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/nix-env-38904d.cpp
clang++: note: diagnostic msg: /tmp/nix-env-38904d.sh
clang++: note: diagnostic msg: 

********************
[11/165] Compiling C++ object tests/unit/liblixexpr-tests.p/libexpr_json.cc.o
FAILED: tests/unit/liblixexpr-tests.p/libexpr_json.cc.o 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Midnight Veil (midnightveil)

<details>
```cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin/clang++ @/tmp/cc-params.CRTKnJ
1.      &lt;invalid&gt;: unknown current parser token
  #<!-- -->0 0x00007f2b046dd2de llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedd2de)
  #<!-- -->1 0x00007f2b046db04c llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedb04c)
  #<!-- -->2 0x00007f2b045cd498 CrashRecoverySignalHandler(int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xdcd498)
  #<!-- -->3 0x00007f2b03247620 __restore_rt (/nix/store/wn7v2vhyyyi6clcyn0s9ixvl7d4d87ic-glibc-2.40-36/lib/libc.so.6+0x40620)
  #<!-- -->4 0x00007f2b0cf7a50c clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb7a50c)
  #<!-- -->5 0x00007f2b0cf8178f clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb8178f)
  #<!-- -->6 0x00007f2b0cfc62a5 clang::Preprocessor::SkipExcludedConditionalBlock(clang::SourceLocation, clang::SourceLocation, bool, bool, clang::SourceLocation) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc62a5)
  #<!-- -->7 0x00007f2b0cfc7dfa clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc7dfa)
  #<!-- -->8 0x00007f2b0cfcd323 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbcd323)
  #<!-- -->9 0x00007f2b0cf831ce clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb831ce)
 #<!-- -->10 0x00007f2b0d0036b4 clang::Preprocessor::Lex(clang::Token&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc036b4)
 #<!-- -->11 0x00007f2b0d02356a clang::Parser::ConsumeAnyToken(bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc2356a)
 #<!-- -->12 0x00007f2b0d1158ba clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd158ba)
// continues

```

preprocessed source and run-script.
[source.zip](https://github.com/user-attachments/files/20737535/source.zip)

Occurred while making modifications https://git.lix.systems/lix-project/lix. I'm not quite sure what happened here, I think somehow the in memory representation became a whole bunch of zeroes and clang got extremely confused. It went away after rebuilding. Suspect this might have something to do with precompiled headers? (gut feeling)


&lt;details&gt;&lt;summary&gt;Entire output&lt;/summary&gt;

```
   62 |     JSON toJSON(Store &amp; store) const;
      |                      ^
../lix/libcmd/cmd-profiles.hh:62:23: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                       ^
../lix/libcmd/cmd-profiles.hh:62:24: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                        ^
../lix/libcmd/cmd-profiles.hh:62:28: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                            ^
../lix/libcmd/cmd-profiles.hh:62:29: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                             ^
../lix/libcmd/cmd-profiles.hh:62:30: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                              ^
../lix/libcmd/cmd-profiles.hh:62:31: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                               ^
../lix/libcmd/cmd-profiles.hh:62:32: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                                ^
../lix/libcmd/cmd-profiles.hh:62:36: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                                    ^
../lix/libcmd/cmd-profiles.hh:62:37: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                                     ^
../lix/libcmd/cmd-profiles.hh:62:38: warning: null character ignored [-Wnull-character]
   62 |     JSON toJSON(Store &amp; store) const;
      |                                      ^
../lix/libcmd/cmd-profiles.hh:63:1: warning: null character ignored [-Wnull-character]
   63 | 
      | ^
../lix/libcmd/cmd-profiles.hh:64:1: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      | ^
../lix/libcmd/cmd-profiles.hh:64:5: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |     ^
../lix/libcmd/cmd-profiles.hh:64:6: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |      ^
../lix/libcmd/cmd-profiles.hh:64:7: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |       ^
../lix/libcmd/cmd-profiles.hh:64:8: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |        ^
../lix/libcmd/cmd-profiles.hh:64:9: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |         ^
../lix/libcmd/cmd-profiles.hh:64:11: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |           ^
../lix/libcmd/cmd-profiles.hh:64:12: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |            ^
../lix/libcmd/cmd-profiles.hh:64:13: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |             ^
../lix/libcmd/cmd-profiles.hh:64:14: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |              ^
../lix/libcmd/cmd-profiles.hh:64:15: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |               ^
../lix/libcmd/cmd-profiles.hh:64:16: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                ^
../lix/libcmd/cmd-profiles.hh:64:17: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                 ^
../lix/libcmd/cmd-profiles.hh:64:19: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                   ^
../lix/libcmd/cmd-profiles.hh:64:20: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                    ^
../lix/libcmd/cmd-profiles.hh:64:21: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                     ^
../lix/libcmd/cmd-profiles.hh:64:22: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                      ^
../lix/libcmd/cmd-profiles.hh:64:23: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                       ^
../lix/libcmd/cmd-profiles.hh:64:24: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                        ^
../lix/libcmd/cmd-profiles.hh:64:25: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                         ^
../lix/libcmd/cmd-profiles.hh:64:27: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                           ^
../lix/libcmd/cmd-profiles.hh:64:28: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                            ^
../lix/libcmd/cmd-profiles.hh:64:29: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                             ^
../lix/libcmd/cmd-profiles.hh:64:30: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                              ^
../lix/libcmd/cmd-profiles.hh:64:31: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                               ^
../lix/libcmd/cmd-profiles.hh:64:32: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                ^
../lix/libcmd/cmd-profiles.hh:64:33: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                 ^
../lix/libcmd/cmd-profiles.hh:64:35: error: source file is not valid UTF-8
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                   ^
../lix/libcmd/cmd-profiles.hh:64:39: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                       ^
../lix/libcmd/cmd-profiles.hh:64:40: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                        ^
../lix/libcmd/cmd-profiles.hh:64:41: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                         ^
../lix/libcmd/cmd-profiles.hh:64:45: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                             ^
../lix/libcmd/cmd-profiles.hh:64:46: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                              ^
../lix/libcmd/cmd-profiles.hh:64:47: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                               ^
../lix/libcmd/cmd-profiles.hh:64:48: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                ^
../lix/libcmd/cmd-profiles.hh:64:49: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                 ^
../lix/libcmd/cmd-profiles.hh:64:53: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                     ^
../lix/libcmd/cmd-profiles.hh:64:54: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                      ^
../lix/libcmd/cmd-profiles.hh:64:55: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                       ^
../lix/libcmd/cmd-profiles.hh:64:56: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                        ^
../lix/libcmd/cmd-profiles.hh:64:57: warning: null character ignored [-Wnull-character]
   64 |     kj::Promise&lt;Result&lt;StorePath&gt;&gt; build(ref&lt;Store&gt; store);
      |                                                         ^
../lix/libcmd/cmd-profiles.hh:65:1: warning: null character ignored [-Wnull-character]
   65 | 
      | ^
../lix/libcmd/cmd-profiles.hh:66:1: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      | ^
../lix/libcmd/cmd-profiles.hh:66:2: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |  ^
../lix/libcmd/cmd-profiles.hh:66:3: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |   ^
../lix/libcmd/cmd-profiles.hh:66:4: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |    ^
../lix/libcmd/cmd-profiles.hh:66:6: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |      ^
../lix/libcmd/cmd-profiles.hh:66:7: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |       ^
../lix/libcmd/cmd-profiles.hh:66:8: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |        ^
../lix/libcmd/cmd-profiles.hh:66:9: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |         ^
../lix/libcmd/cmd-profiles.hh:66:10: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |          ^
../lix/libcmd/cmd-profiles.hh:66:11: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |           ^
../lix/libcmd/cmd-profiles.hh:66:12: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |            ^
../lix/libcmd/cmd-profiles.hh:66:14: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |              ^
../lix/libcmd/cmd-profiles.hh:66:15: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |               ^
../lix/libcmd/cmd-profiles.hh:66:16: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                ^
../lix/libcmd/cmd-profiles.hh:66:17: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                 ^
../lix/libcmd/cmd-profiles.hh:66:18: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                  ^
../lix/libcmd/cmd-profiles.hh:66:19: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                   ^
../lix/libcmd/cmd-profiles.hh:66:20: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                    ^
../lix/libcmd/cmd-profiles.hh:66:22: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                      ^
../lix/libcmd/cmd-profiles.hh:66:23: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                       ^
../lix/libcmd/cmd-profiles.hh:66:24: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                        ^
../lix/libcmd/cmd-profiles.hh:66:25: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                         ^
../lix/libcmd/cmd-profiles.hh:66:26: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                          ^
../lix/libcmd/cmd-profiles.hh:66:27: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                           ^
../lix/libcmd/cmd-profiles.hh:66:28: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                            ^
../lix/libcmd/cmd-profiles.hh:66:30: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                              ^
../lix/libcmd/cmd-profiles.hh:66:34: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                  ^
../lix/libcmd/cmd-profiles.hh:66:35: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                   ^
../lix/libcmd/cmd-profiles.hh:66:36: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                    ^
../lix/libcmd/cmd-profiles.hh:66:37: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                     ^
../lix/libcmd/cmd-profiles.hh:66:40: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                        ^
../lix/libcmd/cmd-profiles.hh:66:41: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                         ^
../lix/libcmd/cmd-profiles.hh:66:42: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                          ^
../lix/libcmd/cmd-profiles.hh:66:43: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                           ^
../lix/libcmd/cmd-profiles.hh:66:44: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                            ^
../lix/libcmd/cmd-profiles.hh:66:45: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                             ^
../lix/libcmd/cmd-profiles.hh:66:48: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                ^
../lix/libcmd/cmd-profiles.hh:66:49: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                 ^
../lix/libcmd/cmd-profiles.hh:66:50: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                  ^
../lix/libcmd/cmd-profiles.hh:66:51: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                   ^
../lix/libcmd/cmd-profiles.hh:66:52: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                    ^
../lix/libcmd/cmd-profiles.hh:66:53: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                     ^
../lix/libcmd/cmd-profiles.hh:66:56: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                        ^
../lix/libcmd/cmd-profiles.hh:66:57: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                         ^
../lix/libcmd/cmd-profiles.hh:66:58: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                          ^
../lix/libcmd/cmd-profiles.hh:66:59: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                           ^
../lix/libcmd/cmd-profiles.hh:66:60: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                            ^
../lix/libcmd/cmd-profiles.hh:66:61: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                             ^
../lix/libcmd/cmd-profiles.hh:66:63: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                               ^
../lix/libcmd/cmd-profiles.hh:66:64: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                ^
../lix/libcmd/cmd-profiles.hh:66:65: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                 ^
../lix/libcmd/cmd-profiles.hh:66:66: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                  ^
../lix/libcmd/cmd-profiles.hh:66:67: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                   ^
../lix/libcmd/cmd-profiles.hh:66:68: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                    ^
../lix/libcmd/cmd-profiles.hh:66:69: error: source file is not valid UTF-8
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                     ^
../lix/libcmd/cmd-profiles.hh:66:71: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                       ^
../lix/libcmd/cmd-profiles.hh:66:72: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                        ^
../lix/libcmd/cmd-profiles.hh:66:73: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                         ^
../lix/libcmd/cmd-profiles.hh:66:74: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                          ^
../lix/libcmd/cmd-profiles.hh:66:75: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                           ^
../lix/libcmd/cmd-profiles.hh:66:76: warning: null character ignored [-Wnull-character]
   66 |     void addElement(std::string_view nameCandidate, ProfileElement element);
      |                                                                            ^
../lix/libcmd/cmd-profiles.hh:67:1: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      | ^
../lix/libcmd/cmd-profiles.hh:67:2: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |  ^
../lix/libcmd/cmd-profiles.hh:67:3: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |   ^
../lix/libcmd/cmd-profiles.hh:67:4: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |    ^
../lix/libcmd/cmd-profiles.hh:67:5: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |     ^
../lix/libcmd/cmd-profiles.hh:67:6: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |      ^
../lix/libcmd/cmd-profiles.hh:67:7: warning: null character ignored [-Wnull-character]
   67 |     void addElement(ProfileElement element);
      |       ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin/clang++ @/tmp/cc-params.CRTKnJ
1.      &lt;invalid&gt;: unknown current parser token
  #<!-- -->0 0x00007f2b046dd2de llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedd2de)
  #<!-- -->1 0x00007f2b046db04c llvm::sys::CleanupOnSignal(unsigned long) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xedb04c)
  #<!-- -->2 0x00007f2b045cd498 CrashRecoverySignalHandler(int) (/nix/store/i7laizikxvx5hi86g98k4v3p7g8s2a7s-llvm-18.1.8-lib/lib/libLLVM.so.18.1+0xdcd498)
  #<!-- -->3 0x00007f2b03247620 __restore_rt (/nix/store/wn7v2vhyyyi6clcyn0s9ixvl7d4d87ic-glibc-2.40-36/lib/libc.so.6+0x40620)
  #<!-- -->4 0x00007f2b0cf7a50c clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb7a50c)
  #<!-- -->5 0x00007f2b0cf8178f clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb8178f)
  #<!-- -->6 0x00007f2b0cfc62a5 clang::Preprocessor::SkipExcludedConditionalBlock(clang::SourceLocation, clang::SourceLocation, bool, bool, clang::SourceLocation) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc62a5)
  #<!-- -->7 0x00007f2b0cfc7dfa clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbc7dfa)
  #<!-- -->8 0x00007f2b0cfcd323 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xbcd323)
  #<!-- -->9 0x00007f2b0cf831ce clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xb831ce)
 #<!-- -->10 0x00007f2b0d0036b4 clang::Preprocessor::Lex(clang::Token&amp;) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc036b4)
 #<!-- -->11 0x00007f2b0d02356a clang::Parser::ConsumeAnyToken(bool) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xc2356a)
 #<!-- -->12 0x00007f2b0d1158ba clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd158ba)
 #<!-- -->13 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->14 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->15 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->16 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->17 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->18 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 #<!-- -->19 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
 // repeated another 200 times
#<!-- -->252 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#<!-- -->253 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#<!-- -->254 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
#<!-- -->255 0x00007f2b0d1159f8 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) (.constprop.0) (/nix/store/2j35g928qczhj13kfi7yxm09wg2jiv14-clang-18.1.8-lib/lib/libclang-cpp.so.18.1+0xd159f8)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /nix/store/im1yhwr0p1j7r5mi47cyn4sq3wmhzkzf-clang-18.1.8/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/nix-env-38904d.cpp
clang++: note: diagnostic msg: /tmp/nix-env-38904d.sh
clang++: note: diagnostic msg: 

********************
[11/165] Compiling C++ object tests/unit/liblixexpr-tests.p/libexpr_json.cc.o
FAILED: tests/unit/liblixexpr-tests.p/libexpr_json.cc.o 
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 20 or `main` branch? https://godbolt.org should be helpful.


---

### Comment 3 - midnightveil

> Could you please try 20 or main branch? [godbolt.org](https://godbolt.org/) should be helpful.

This was not a reproducible error, rebuilding (un-cleanly) made it go away.

I can upgrade to newer clang and wait.

---

### Comment 4 - zwuis

It looks like a duplicate of #12110.

---

### Comment 5 - midnightveil

Yeah, or #64576.

---

