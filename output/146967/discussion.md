# [HLSL][RootSignature] Update `RootSignatureParser` to output correct `SourceLocation`

**Author:** inbelic
**URL:** https://github.com/llvm/llvm-project/issues/146967
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-07-08T16:55:52Z

## Body

The `SourceLocation` of a `RootSignatureToken` is incorrectly set when the root signature is defined in a multi-line macro expansion.

The root signature attribute is defined with a `StringLiteral` argument. In `ParseDeclCXX.cpp` we retrieve the `StringLiteral` by invoking `ParseUnevaluatedStringLiteralExpression`, which will concatenate each of the lines into a singular string.

Currently, we only pass a `StringRef` of the concatenated strings to the `RootSignatureLexer`. Which means the `SourceLocation` that is used to construct the `RootSignatureTokens` are actually just an offset into the concatenated string. Thus, when we produce the diagnostic, the `SourceLocation` will be "earlier" than the proper location as it did not account for any characters between the end of this token and the start of the next.

This can be resolved by retaining the `SourceLocation` information that is kept in `StringLiteral` and then converting the offset in the concatenated string into the proper `SourceLocation` using the `StringLiteral::getLocationOfByte` interface. To do so, we will need to adjust the `RootSignatureToken` to only hold its offset into the root signature string. Then when the parser will use the token, it will need to compute its actual `SourceLocation`.

Note: Since the `Lex` library does not have a dependency on `AST`, which contains the definitions of `StringLiteral`, the design decision is to let the Parser handle computation of the `SourceLocation` when it will output its diagnostics. This will allow the `RootSignatureLexer` to stay very simple.

For example:
https://godbolt.org/z/WsPf599c7

```
#define DemoRootSignature \
 "CBV(b0)," \
 "RootConstants(num32BitConstants = 3, b0, invalid)"
  expected caret location --------------------^
  actual caret location ------------------^
```

The caret points 5 characters early because the current offset did not account for the characters:
```
'"' ' ' '\' ' ' '"'
 1   2   3   4   5
```

AC:
- [ ] Update `RootSignatureParser` to retain `SourceLocation` information by retaining the `StringLiteral` and passing the underlying `StringRef` to the `Lexer`
- [ ] Update `RootSignatureLexer` so that the constructed tokens only reflect an offset into the `StringRef`
- [ ] Update `RootSignatureParser` to use `StringLiteral::getLocationOfByte` to get the actual token location for diagnostics
- [ ] Update `ParseHLSLRootSignatureTest` to construct a phony `AST`/`StringLiteral` for the test cases
- [ ] Add a test to `RootSignature-err.hlsl` showing that the `SourceLocation` is correctly set for diagnostics in a multi-line macro expansion

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Finn Plummer (inbelic)

<details>
The `SourceLocation` of a `RootSignatureToken` is incorrectly set when the root signature is defined in a multi-line macro expansion.

The root signature attribute is defined with a `StringLiteral` argument. In `ParseDeclCXX.cpp` we retrieve the `StringLiteral` by invoking `ParseUnevaluatedStringLiteralExpression`, which will concatenate each of the lines into a singular string.

Currently, we only pass a `StringRef` of the concatenated strings to the `RootSignatureLexer`. Which means the `SourceLocation` that is used to construct the `RootSignatureTokens` are actually just an offset into the concatenated string. Thus, when we produce the diagnostic, the `SourceLocation` will be "earlier" than the proper location as it did not account for any characters between the end of this token and the start of the next.

This can be resolved by retaining the `SourceLocation` information that is kept in `StringLiteral` and then converting the offset in the concatenated string into the proper `SourceLocation` using the `StringLiteral::getLocationOfByte` interface. To do so, we will need to adjust the `RootSignatureToken` to only hold its offset into the root signature string. Then when the parser will use the token, it will need to compute its actual `SourceLocation`.

Note: Since the `Lex` library does not have a dependency on `AST`, which contains the definitions of `StringLiteral`, the design decision is to let the Parser handle computation of the `SourceLocation` when it will output its diagnostics. This will allow the `RootSignatureLexer` to stay very simple.

For example:
https://godbolt.org/z/WsPf599c7

```
#define DemoRootSignature \
 "CBV(b0)," \
 "RootConstants(num32BitConstants = 3, b0, invalid)"
  expected caret location --------------------^
  actual caret location ------------------^
```

The caret points 5 characters early because the current offset did not account for the characters:
```
'"' ' ' '\' ' ' '"'
 1   2   3   4   5
```

AC:
- [ ] Update `RootSignatureParser` to retain `SourceLocation` information by retaining the `StringLiteral` and passing the underlying `StringRef` to the `Lexer`
- [ ] Update `RootSignatureLexer` so that the constructed tokens only reflect an offset into the `StringRef`
- [ ] Update `RootSignatureParser` to use `StringLiteral::getLocationOfByte` to get the actual token location for diagnostics
- [ ] Update `ParseHLSLRootSignatureTest` to construct a phony `AST`/`StringLiteral` for the test cases
- [ ] Add a test to `RootSignature-err.hlsl` showing that the `SourceLocation` is correctly set for diagnostics in a multi-line macro expansion
</details>


---

