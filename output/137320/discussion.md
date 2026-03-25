# Crash during clang ast-dump, but only in JSON output mode

**Author:** dalemyers
**URL:** https://github.com/llvm/llvm-project/issues/137320
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, objective-c
**Closed Date:** 2025-04-29T11:57:05Z

## Body

I've been running into a seg fault when trying to dump an AST as JSON. Here's the minimal repro code:

```objc
@interface SomeClass (SomeExtension)
+ (void)someMethod;
@end
```

It must be an interface, it must be an extension, and it must have at least one method in the extension. Any changes to that and the seg fault doesn't happen. 

Here's the output:

<details>
  <summary>Expand</summary>
```
$ clang -cc1 -ast-dump=json 'Example.m'
/path/to/Example.m:1:12: error: cannot find interface declaration for 'SomeClass'
    1 | @interface SomeClass (SomeExtension)
      |            ^
{
  "id": "0x13b840408",
  "kind": "TranslationUnitDecl",
  "loc": {},
  "range": {
    "begin": {},
    "end": {}
  },
  "inner": [
    {
      "id": "0x13b8412b0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__int128_t",
      "type": {
        "qualType": "__int128"
      },
      "inner": [
        {
          "id": "0x13b8409d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__int128"
          }
        }
      ]
    },
    {
      "id": "0x13b841320",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__uint128_t",
      "type": {
        "qualType": "unsigned __int128"
      },
      "inner": [
        {
          "id": "0x13b8409f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "unsigned __int128"
          }
        }
      ]
    },
    {
      "id": "0x13b905a00",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "SEL",
      "type": {
        "qualType": "SEL *"
      },
      "inner": [
        {
          "id": "0x13b841380",
          "kind": "PointerType",
          "type": {
            "qualType": "SEL *"
          },
          "inner": [
            {
              "id": "0x13b840b70",
              "kind": "BuiltinType",
              "type": {
                "qualType": "SEL"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905ae8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "id",
      "type": {
        "qualType": "id"
      },
      "inner": [
        {
          "id": "0x13b905a90",
          "kind": "ObjCObjectPointerType",
          "type": {
            "qualType": "id"
          },
          "inner": [
            {
              "id": "0x13b905a60",
              "kind": "ObjCObjectType",
              "type": {
                "qualType": "id"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905bc8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "Class",
      "type": {
        "qualType": "Class"
      },
      "inner": [
        {
          "id": "0x13b905b70",
          "kind": "ObjCObjectPointerType",
          "type": {
            "qualType": "Class"
          },
          "inner": [
            {
              "id": "0x13b905b40",
              "kind": "ObjCObjectType",
              "type": {
                "qualType": "Class"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905c20",
      "kind": "ObjCInterfaceDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "Protocol",
      "mangledName": "_OBJC_CLASS_$_Protocol",
      "super": {
        "id": "0x0"
      },
      "implementation": {
        "id": "0x0"
      }
    },
    {
      "id": "0x13b905f80",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__NSConstantString",
      "type": {
        "qualType": "struct __NSConstantString_tag"
      },
      "inner": [
        {
          "id": "0x13b905d50",
          "kind": "RecordType",
          "type": {
            "qualType": "struct __NSConstantString_tag"
          },
          "decl": {
            "id": "0x13b905cd0",
            "kind": "RecordDecl",
            "name": "__NSConstantString_tag"
          }
        }
      ]
    },
    {
      "id": "0x13b905fe8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt8_t",
      "type": {
        "qualType": "__SVInt8_t"
      },
      "inner": [
        {
          "id": "0x13b840b90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt8_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906050",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt16_t",
      "type": {
        "qualType": "__SVInt16_t"
      },
      "inner": [
        {
          "id": "0x13b840bb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9060b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt32_t",
      "type": {
        "qualType": "__SVInt32_t"
      },
      "inner": [
        {
          "id": "0x13b840bd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906120",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt64_t",
      "type": {
        "qualType": "__SVInt64_t"
      },
      "inner": [
        {
          "id": "0x13b840bf0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906188",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint8_t",
      "type": {
        "qualType": "__SVUint8_t"
      },
      "inner": [
        {
          "id": "0x13b840c10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint8_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9061f0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint16_t",
      "type": {
        "qualType": "__SVUint16_t"
      },
      "inner": [
        {
          "id": "0x13b840c30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906258",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint32_t",
      "type": {
        "qualType": "__SVUint32_t"
      },
      "inner": [
        {
          "id": "0x13b840c50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9062c0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint64_t",
      "type": {
        "qualType": "__SVUint64_t"
      },
      "inner": [
        {
          "id": "0x13b840c70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906328",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat16_t",
      "type": {
        "qualType": "__SVFloat16_t"
      },
      "inner": [
        {
          "id": "0x13b840c90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906390",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat32_t",
      "type": {
        "qualType": "__SVFloat32_t"
      },
      "inner": [
        {
          "id": "0x13b840cb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9063f8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat64_t",
      "type": {
        "qualType": "__SVFloat64_t"
      },
      "inner": [
        {
          "id": "0x13b840cd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906460",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVBFloat16_t",
      "type": {
        "qualType": "__SVBFloat16_t"
      },
      "inner": [
        {
          "id": "0x13b840cf0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVBFloat16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9064c8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x2_t",
      "type": {
        "qualType": "__clang_svint8x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906530",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x2_t",
      "type": {
        "qualType": "__clang_svint16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906598",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x2_t",
      "type": {
        "qualType": "__clang_svint32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906600",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x2_t",
      "type": {
        "qualType": "__clang_svint64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906668",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x2_t",
      "type": {
        "qualType": "__clang_svuint8x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9066d0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x2_t",
      "type": {
        "qualType": "__clang_svuint16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840db0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906738",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x2_t",
      "type": {
        "qualType": "__clang_svuint32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840dd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9067a0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x2_t",
      "type": {
        "qualType": "__clang_svuint64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840df0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906808",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x2_t",
      "type": {
        "qualType": "__clang_svfloat16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906870",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x2_t",
      "type": {
        "qualType": "__clang_svfloat32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9068d8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x2_t",
      "type": {
        "qualType": "__clang_svfloat64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906940",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x2_t",
      "type": {
        "qualType": "__clang_svbfloat16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9069a8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x3_t",
      "type": {
        "qualType": "__clang_svint8x3_t"
      },
      "inner": [
        {
          "id": "0x13b840e90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906e10",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x3_t",
      "type": {
        "qualType": "__clang_svint16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840eb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906e78",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x3_t",
      "type": {
        "qualType": "__clang_svint32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ed0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906ee0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x3_t",
      "type": {
        "qualType": "__clang_svint64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ef0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906f48",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x3_t",
      "type": {
        "qualType": "__clang_svuint8x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906fb0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x3_t",
      "type": {
        "qualType": "__clang_svuint16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907018",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x3_t",
      "type": {
        "qualType": "__clang_svuint32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907080",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x3_t",
      "type": {
        "qualType": "__clang_svuint64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9070e8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x3_t",
      "type": {
        "qualType": "__clang_svfloat16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907150",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x3_t",
      "type": {
        "qualType": "__clang_svfloat32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840fb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9071b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x3_t",
      "type": {
        "qualType": "__clang_svfloat64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840fd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907220",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x3_t",
      "type": {
        "qualType": "__clang_svbfloat16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ff0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907288",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x4_t",
      "type": {
        "qualType": "__clang_svint8x4_t"
      },
      "inner": [
        {
          "id": "0x13b841010",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9072f0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x4_t",
      "type": {
        "qualType": "__clang_svint16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841030",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907358",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x4_t",
      "type": {
        "qualType": "__clang_svint32x4_t"
      },
      "inner": [
        {
          "id": "0x13b841050",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9073c0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x4_t",
      "type": {
        "qualType": "__clang_svint64x4_t"
      },
      "inner": [
        {
          "id": "0x13b841070",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907428",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x4_t",
      "type": {
        "qualType": "__clang_svuint8x4_t"
      },
      "inner": [
        {
          "id": "0x13b841090",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907490",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x4_t",
      "type": {
        "qualType": "__clang_svuint16x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410b0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9074f8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x4_t",
      "type": {
        "qualType": "__clang_svuint32x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907560",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x4_t",
      "type": {
        "qualType": "__clang_svuint64x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9075c8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x4_t",
      "type": {
        "qualType": "__clang_svfloat16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841110",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907630",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x4_t",
      "type": {
        "qualType": "__clang_svfloat32x4_t"
      },
      "inner": [
        {
          "id": "0x13b841130",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907698",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x4_t",
      "type": {
        "qualType": "__clang_svfloat64x4_t"
      },
      "inner": [
        {
          "id": "0x13b841150",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907700",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x4_t",
      "type": {
        "qualType": "__clang_svbfloat16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841170",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907768",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVBool_t",
      "type": {
        "qualType": "__SVBool_t"
      },
      "inner": [
        {
          "id": "0x13b841190",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVBool_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9077d0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svboolx2_t",
      "type": {
        "qualType": "__clang_svboolx2_t"
      },
      "inner": [
        {
          "id": "0x13b8411b0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svboolx2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907838",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svboolx4_t",
      "type": {
        "qualType": "__clang_svboolx4_t"
      },
      "inner": [
        {
          "id": "0x13b8411d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svboolx4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9078a0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVCount_t",
      "type": {
        "qualType": "__SVCount_t"
      },
      "inner": [
        {
          "id": "0x13b8411f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVCount_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907948",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__builtin_ms_va_list",
      "type": {
        "qualType": "char *"
      },
      "inner": [
        {
          "id": "0x13b907900",
          "kind": "PointerType",
          "type": {
            "qualType": "char *"
          },
          "inner": [
            {
              "id": "0x13b8404b0",
              "kind": "BuiltinType",
              "type": {
                "qualType": "char"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b9079b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__builtin_va_list",
      "type": {
        "qualType": "char *"
      },
      "inner": [
        {
          "id": "0x13b907900",
          "kind": "PointerType",
          "type": {
            "qualType": "char *"
          },
          "inner": [
            {
              "id": "0x13b8404b0",
              "kind": "BuiltinType",
              "type": {
                "qualType": "char"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b907a28",
      "kind": "ObjCCategoryDecl",
      "loc": {
        "offset": 11,
        "file": "/path/to/Example.m",
        "line": 1,
        "col": 12,
        "tokLen": 9
      },
      "range": {
        "begin": {
          "offset": 0,
          "col": 1,
          "tokLen": 1
        },
        "end": {
          "offset": 49,
          "line": 3,
          "col": 2,
          "tokLen": 3
        }
      },
      "isInvalid": true,
      "name": "SomeExtension",
      "interface": {
        "id": "0x0"
      },
      "implementation": {
        "id": "0x0"
      },
      "inner": [
        {
          "id": "0x13b907b88",
          "kind": "ObjCMethodDecl",
          "loc": {
            "offset": 37,
            "line": 2,
            "col": 1,
            "tokLen": 1
          },
          "range": {
            "begin": {
              "offset": 37,
              "col": 1,
              "tokLen": 1
            },
            "end": {
              "offset": 46,
              "col": 10,
              "tokLen": 1
            }
          },
          "name": "f"[1]    35259 segmentation fault  clang -cc1 -ast-dump=json
```
</details>

And here's what it gives when running in text mode:

<details>
  <summary>Expand</summary>
```
$ clang -cc1 -ast-dump 'Example.m'
/path/to/Example.m:1:12: error: cannot find interface declaration for 'SomeClass'
    1 | @interface SomeClass (SomeExtension)
      |            ^
TranslationUnitDecl 0x12c040408 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x12c0412b0 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0x12c0409d0 '__int128'
|-TypedefDecl 0x12c041320 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x12c0409f0 'unsigned __int128'
|-TypedefDecl 0x12c105a00 <<invalid sloc>> <invalid sloc> implicit SEL 'SEL *'
| `-PointerType 0x12c041380 'SEL *'
|   `-BuiltinType 0x12c040b70 'SEL'
|-TypedefDecl 0x12c105ae8 <<invalid sloc>> <invalid sloc> implicit id 'id'
| `-ObjCObjectPointerType 0x12c105a90 'id'
|   `-ObjCObjectType 0x12c105a60 'id'
|-TypedefDecl 0x12c105bc8 <<invalid sloc>> <invalid sloc> implicit Class 'Class'
| `-ObjCObjectPointerType 0x12c105b70 'Class'
|   `-ObjCObjectType 0x12c105b40 'Class'
|-ObjCInterfaceDecl 0x12c105c20 <<invalid sloc>> <invalid sloc> implicit Protocol
|-TypedefDecl 0x12c105f80 <<invalid sloc>> <invalid sloc> implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0x12c105d50 'struct __NSConstantString_tag'
|   `-Record 0x12c105cd0 '__NSConstantString_tag'
|-TypedefDecl 0x12c105fe8 <<invalid sloc>> <invalid sloc> implicit __SVInt8_t '__SVInt8_t'
| `-BuiltinType 0x12c040b90 '__SVInt8_t'
|-TypedefDecl 0x12c106050 <<invalid sloc>> <invalid sloc> implicit __SVInt16_t '__SVInt16_t'
| `-BuiltinType 0x12c040bb0 '__SVInt16_t'
|-TypedefDecl 0x12c1060b8 <<invalid sloc>> <invalid sloc> implicit __SVInt32_t '__SVInt32_t'
| `-BuiltinType 0x12c040bd0 '__SVInt32_t'
|-TypedefDecl 0x12c106120 <<invalid sloc>> <invalid sloc> implicit __SVInt64_t '__SVInt64_t'
| `-BuiltinType 0x12c040bf0 '__SVInt64_t'
|-TypedefDecl 0x12c106188 <<invalid sloc>> <invalid sloc> implicit __SVUint8_t '__SVUint8_t'
| `-BuiltinType 0x12c040c10 '__SVUint8_t'
|-TypedefDecl 0x12c1061f0 <<invalid sloc>> <invalid sloc> implicit __SVUint16_t '__SVUint16_t'
| `-BuiltinType 0x12c040c30 '__SVUint16_t'
|-TypedefDecl 0x12c106258 <<invalid sloc>> <invalid sloc> implicit __SVUint32_t '__SVUint32_t'
| `-BuiltinType 0x12c040c50 '__SVUint32_t'
|-TypedefDecl 0x12c1062c0 <<invalid sloc>> <invalid sloc> implicit __SVUint64_t '__SVUint64_t'
| `-BuiltinType 0x12c040c70 '__SVUint64_t'
|-TypedefDecl 0x12c106328 <<invalid sloc>> <invalid sloc> implicit __SVFloat16_t '__SVFloat16_t'
| `-BuiltinType 0x12c040c90 '__SVFloat16_t'
|-TypedefDecl 0x12c106390 <<invalid sloc>> <invalid sloc> implicit __SVFloat32_t '__SVFloat32_t'
| `-BuiltinType 0x12c040cb0 '__SVFloat32_t'
|-TypedefDecl 0x12c1063f8 <<invalid sloc>> <invalid sloc> implicit __SVFloat64_t '__SVFloat64_t'
| `-BuiltinType 0x12c040cd0 '__SVFloat64_t'
|-TypedefDecl 0x12c106460 <<invalid sloc>> <invalid sloc> implicit __SVBFloat16_t '__SVBFloat16_t'
| `-BuiltinType 0x12c040cf0 '__SVBFloat16_t'
|-TypedefDecl 0x12c1064c8 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x2_t '__clang_svint8x2_t'
| `-BuiltinType 0x12c040d10 '__clang_svint8x2_t'
|-TypedefDecl 0x12c106530 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x2_t '__clang_svint16x2_t'
| `-BuiltinType 0x12c040d30 '__clang_svint16x2_t'
|-TypedefDecl 0x12c106598 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x2_t '__clang_svint32x2_t'
| `-BuiltinType 0x12c040d50 '__clang_svint32x2_t'
|-TypedefDecl 0x12c106600 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x2_t '__clang_svint64x2_t'
| `-BuiltinType 0x12c040d70 '__clang_svint64x2_t'
|-TypedefDecl 0x12c106668 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x2_t '__clang_svuint8x2_t'
| `-BuiltinType 0x12c040d90 '__clang_svuint8x2_t'
|-TypedefDecl 0x12c1066d0 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x2_t '__clang_svuint16x2_t'
| `-BuiltinType 0x12c040db0 '__clang_svuint16x2_t'
|-TypedefDecl 0x12c106738 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x2_t '__clang_svuint32x2_t'
| `-BuiltinType 0x12c040dd0 '__clang_svuint32x2_t'
|-TypedefDecl 0x12c1067a0 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x2_t '__clang_svuint64x2_t'
| `-BuiltinType 0x12c040df0 '__clang_svuint64x2_t'
|-TypedefDecl 0x12c106808 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x2_t '__clang_svfloat16x2_t'
| `-BuiltinType 0x12c040e10 '__clang_svfloat16x2_t'
|-TypedefDecl 0x12c106870 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x2_t '__clang_svfloat32x2_t'
| `-BuiltinType 0x12c040e30 '__clang_svfloat32x2_t'
|-TypedefDecl 0x12c1068d8 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x2_t '__clang_svfloat64x2_t'
| `-BuiltinType 0x12c040e50 '__clang_svfloat64x2_t'
|-TypedefDecl 0x12c106940 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x2_t '__clang_svbfloat16x2_t'
| `-BuiltinType 0x12c040e70 '__clang_svbfloat16x2_t'
|-TypedefDecl 0x12c1069a8 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x3_t '__clang_svint8x3_t'
| `-BuiltinType 0x12c040e90 '__clang_svint8x3_t'
|-TypedefDecl 0x12c106e10 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x3_t '__clang_svint16x3_t'
| `-BuiltinType 0x12c040eb0 '__clang_svint16x3_t'
|-TypedefDecl 0x12c106e78 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x3_t '__clang_svint32x3_t'
| `-BuiltinType 0x12c040ed0 '__clang_svint32x3_t'
|-TypedefDecl 0x12c106ee0 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x3_t '__clang_svint64x3_t'
| `-BuiltinType 0x12c040ef0 '__clang_svint64x3_t'
|-TypedefDecl 0x12c106f48 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x3_t '__clang_svuint8x3_t'
| `-BuiltinType 0x12c040f10 '__clang_svuint8x3_t'
|-TypedefDecl 0x12c106fb0 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x3_t '__clang_svuint16x3_t'
| `-BuiltinType 0x12c040f30 '__clang_svuint16x3_t'
|-TypedefDecl 0x12c107018 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x3_t '__clang_svuint32x3_t'
| `-BuiltinType 0x12c040f50 '__clang_svuint32x3_t'
|-TypedefDecl 0x12c107080 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x3_t '__clang_svuint64x3_t'
| `-BuiltinType 0x12c040f70 '__clang_svuint64x3_t'
|-TypedefDecl 0x12c1070e8 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x3_t '__clang_svfloat16x3_t'
| `-BuiltinType 0x12c040f90 '__clang_svfloat16x3_t'
|-TypedefDecl 0x12c107150 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x3_t '__clang_svfloat32x3_t'
| `-BuiltinType 0x12c040fb0 '__clang_svfloat32x3_t'
|-TypedefDecl 0x12c1071b8 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x3_t '__clang_svfloat64x3_t'
| `-BuiltinType 0x12c040fd0 '__clang_svfloat64x3_t'
|-TypedefDecl 0x12c107220 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x3_t '__clang_svbfloat16x3_t'
| `-BuiltinType 0x12c040ff0 '__clang_svbfloat16x3_t'
|-TypedefDecl 0x12c107288 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x4_t '__clang_svint8x4_t'
| `-BuiltinType 0x12c041010 '__clang_svint8x4_t'
|-TypedefDecl 0x12c1072f0 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x4_t '__clang_svint16x4_t'
| `-BuiltinType 0x12c041030 '__clang_svint16x4_t'
|-TypedefDecl 0x12c107358 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x4_t '__clang_svint32x4_t'
| `-BuiltinType 0x12c041050 '__clang_svint32x4_t'
|-TypedefDecl 0x12c1073c0 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x4_t '__clang_svint64x4_t'
| `-BuiltinType 0x12c041070 '__clang_svint64x4_t'
|-TypedefDecl 0x12c107428 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x4_t '__clang_svuint8x4_t'
| `-BuiltinType 0x12c041090 '__clang_svuint8x4_t'
|-TypedefDecl 0x12c107490 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x4_t '__clang_svuint16x4_t'
| `-BuiltinType 0x12c0410b0 '__clang_svuint16x4_t'
|-TypedefDecl 0x12c1074f8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x4_t '__clang_svuint32x4_t'
| `-BuiltinType 0x12c0410d0 '__clang_svuint32x4_t'
|-TypedefDecl 0x12c107560 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x4_t '__clang_svuint64x4_t'
| `-BuiltinType 0x12c0410f0 '__clang_svuint64x4_t'
|-TypedefDecl 0x12c1075c8 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x4_t '__clang_svfloat16x4_t'
| `-BuiltinType 0x12c041110 '__clang_svfloat16x4_t'
|-TypedefDecl 0x12c107630 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x4_t '__clang_svfloat32x4_t'
| `-BuiltinType 0x12c041130 '__clang_svfloat32x4_t'
|-TypedefDecl 0x12c107698 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x4_t '__clang_svfloat64x4_t'
| `-BuiltinType 0x12c041150 '__clang_svfloat64x4_t'
|-TypedefDecl 0x12c107700 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x4_t '__clang_svbfloat16x4_t'
| `-BuiltinType 0x12c041170 '__clang_svbfloat16x4_t'
|-TypedefDecl 0x12c107768 <<invalid sloc>> <invalid sloc> implicit __SVBool_t '__SVBool_t'
| `-BuiltinType 0x12c041190 '__SVBool_t'
|-TypedefDecl 0x12c1077d0 <<invalid sloc>> <invalid sloc> implicit __clang_svboolx2_t '__clang_svboolx2_t'
| `-BuiltinType 0x12c0411b0 '__clang_svboolx2_t'
|-TypedefDecl 0x12c107838 <<invalid sloc>> <invalid sloc> implicit __clang_svboolx4_t '__clang_svboolx4_t'
| `-BuiltinType 0x12c0411d0 '__clang_svboolx4_t'
|-TypedefDecl 0x12c1078a0 <<invalid sloc>> <invalid sloc> implicit __SVCount_t '__SVCount_t'
| `-BuiltinType 0x12c0411f0 '__SVCount_t'
|-TypedefDecl 0x12c107948 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x12c107900 'char *'
|   `-BuiltinType 0x12c0404b0 'char'
|-TypedefDecl 0x12c1079b8 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list 'char *'
| `-PointerType 0x12c107900 'char *'
|   `-BuiltinType 0x12c0404b0 'char'
`-ObjCCategoryDecl 0x12c107a28 </path/to/Example.m:1:1, line:3:2> line:1:12 invalid SomeExtension
  `-ObjCMethodDecl 0x12c107b88 <line:2:1, col:10> col:1 + f 'void'
1 error generated.
```
</details>

I'm on a mac running macOS 15.4. 

`clang -cc1 -version`:

```
Apple LLVM version 16.0.0
   (clang-1600.0.26.6)Optimized build.
```



## Comments

### Comment 1 - AaronBallman

https://godbolt.org/z/87nGePEnh shows the assertion

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dale (dalemyers)

<details>
I've been running into a seg fault when trying to dump an AST as JSON. Here's the minimal repro code:

```objc
@<!-- -->interface SomeClass (SomeExtension)
+ (void)someMethod;
@<!-- -->end
```

It must be an interface, it must be an extension, and it must have at least one method in the extension. Any changes to that and the seg fault doesn't happen. 

Here's the output:

&lt;details&gt;
  &lt;summary&gt;Expand&lt;/summary&gt;
```
$ clang -cc1 -ast-dump=json 'Example.m'
/path/to/Example.m:1:12: error: cannot find interface declaration for 'SomeClass'
    1 | @<!-- -->interface SomeClass (SomeExtension)
      |            ^
{
  "id": "0x13b840408",
  "kind": "TranslationUnitDecl",
  "loc": {},
  "range": {
    "begin": {},
    "end": {}
  },
  "inner": [
    {
      "id": "0x13b8412b0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__int128_t",
      "type": {
        "qualType": "__int128"
      },
      "inner": [
        {
          "id": "0x13b8409d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__int128"
          }
        }
      ]
    },
    {
      "id": "0x13b841320",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__uint128_t",
      "type": {
        "qualType": "unsigned __int128"
      },
      "inner": [
        {
          "id": "0x13b8409f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "unsigned __int128"
          }
        }
      ]
    },
    {
      "id": "0x13b905a00",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "SEL",
      "type": {
        "qualType": "SEL *"
      },
      "inner": [
        {
          "id": "0x13b841380",
          "kind": "PointerType",
          "type": {
            "qualType": "SEL *"
          },
          "inner": [
            {
              "id": "0x13b840b70",
              "kind": "BuiltinType",
              "type": {
                "qualType": "SEL"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905ae8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "id",
      "type": {
        "qualType": "id"
      },
      "inner": [
        {
          "id": "0x13b905a90",
          "kind": "ObjCObjectPointerType",
          "type": {
            "qualType": "id"
          },
          "inner": [
            {
              "id": "0x13b905a60",
              "kind": "ObjCObjectType",
              "type": {
                "qualType": "id"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905bc8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "Class",
      "type": {
        "qualType": "Class"
      },
      "inner": [
        {
          "id": "0x13b905b70",
          "kind": "ObjCObjectPointerType",
          "type": {
            "qualType": "Class"
          },
          "inner": [
            {
              "id": "0x13b905b40",
              "kind": "ObjCObjectType",
              "type": {
                "qualType": "Class"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b905c20",
      "kind": "ObjCInterfaceDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "Protocol",
      "mangledName": "_OBJC_CLASS_$_Protocol",
      "super": {
        "id": "0x0"
      },
      "implementation": {
        "id": "0x0"
      }
    },
    {
      "id": "0x13b905f80",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__NSConstantString",
      "type": {
        "qualType": "struct __NSConstantString_tag"
      },
      "inner": [
        {
          "id": "0x13b905d50",
          "kind": "RecordType",
          "type": {
            "qualType": "struct __NSConstantString_tag"
          },
          "decl": {
            "id": "0x13b905cd0",
            "kind": "RecordDecl",
            "name": "__NSConstantString_tag"
          }
        }
      ]
    },
    {
      "id": "0x13b905fe8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt8_t",
      "type": {
        "qualType": "__SVInt8_t"
      },
      "inner": [
        {
          "id": "0x13b840b90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt8_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906050",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt16_t",
      "type": {
        "qualType": "__SVInt16_t"
      },
      "inner": [
        {
          "id": "0x13b840bb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9060b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt32_t",
      "type": {
        "qualType": "__SVInt32_t"
      },
      "inner": [
        {
          "id": "0x13b840bd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906120",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVInt64_t",
      "type": {
        "qualType": "__SVInt64_t"
      },
      "inner": [
        {
          "id": "0x13b840bf0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVInt64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906188",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint8_t",
      "type": {
        "qualType": "__SVUint8_t"
      },
      "inner": [
        {
          "id": "0x13b840c10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint8_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9061f0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint16_t",
      "type": {
        "qualType": "__SVUint16_t"
      },
      "inner": [
        {
          "id": "0x13b840c30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906258",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint32_t",
      "type": {
        "qualType": "__SVUint32_t"
      },
      "inner": [
        {
          "id": "0x13b840c50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9062c0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVUint64_t",
      "type": {
        "qualType": "__SVUint64_t"
      },
      "inner": [
        {
          "id": "0x13b840c70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVUint64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906328",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat16_t",
      "type": {
        "qualType": "__SVFloat16_t"
      },
      "inner": [
        {
          "id": "0x13b840c90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906390",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat32_t",
      "type": {
        "qualType": "__SVFloat32_t"
      },
      "inner": [
        {
          "id": "0x13b840cb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat32_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9063f8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVFloat64_t",
      "type": {
        "qualType": "__SVFloat64_t"
      },
      "inner": [
        {
          "id": "0x13b840cd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVFloat64_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906460",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVBFloat16_t",
      "type": {
        "qualType": "__SVBFloat16_t"
      },
      "inner": [
        {
          "id": "0x13b840cf0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVBFloat16_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9064c8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x2_t",
      "type": {
        "qualType": "__clang_svint8x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906530",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x2_t",
      "type": {
        "qualType": "__clang_svint16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906598",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x2_t",
      "type": {
        "qualType": "__clang_svint32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906600",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x2_t",
      "type": {
        "qualType": "__clang_svint64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906668",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x2_t",
      "type": {
        "qualType": "__clang_svuint8x2_t"
      },
      "inner": [
        {
          "id": "0x13b840d90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9066d0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x2_t",
      "type": {
        "qualType": "__clang_svuint16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840db0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906738",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x2_t",
      "type": {
        "qualType": "__clang_svuint32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840dd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9067a0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x2_t",
      "type": {
        "qualType": "__clang_svuint64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840df0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906808",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x2_t",
      "type": {
        "qualType": "__clang_svfloat16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906870",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x2_t",
      "type": {
        "qualType": "__clang_svfloat32x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9068d8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x2_t",
      "type": {
        "qualType": "__clang_svfloat64x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906940",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x2_t",
      "type": {
        "qualType": "__clang_svbfloat16x2_t"
      },
      "inner": [
        {
          "id": "0x13b840e70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9069a8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x3_t",
      "type": {
        "qualType": "__clang_svint8x3_t"
      },
      "inner": [
        {
          "id": "0x13b840e90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906e10",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x3_t",
      "type": {
        "qualType": "__clang_svint16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840eb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906e78",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x3_t",
      "type": {
        "qualType": "__clang_svint32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ed0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906ee0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x3_t",
      "type": {
        "qualType": "__clang_svint64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ef0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906f48",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x3_t",
      "type": {
        "qualType": "__clang_svuint8x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f10",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b906fb0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x3_t",
      "type": {
        "qualType": "__clang_svuint16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f30",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907018",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x3_t",
      "type": {
        "qualType": "__clang_svuint32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f50",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907080",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x3_t",
      "type": {
        "qualType": "__clang_svuint64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f70",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9070e8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x3_t",
      "type": {
        "qualType": "__clang_svfloat16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840f90",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907150",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x3_t",
      "type": {
        "qualType": "__clang_svfloat32x3_t"
      },
      "inner": [
        {
          "id": "0x13b840fb0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9071b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x3_t",
      "type": {
        "qualType": "__clang_svfloat64x3_t"
      },
      "inner": [
        {
          "id": "0x13b840fd0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907220",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x3_t",
      "type": {
        "qualType": "__clang_svbfloat16x3_t"
      },
      "inner": [
        {
          "id": "0x13b840ff0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x3_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907288",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint8x4_t",
      "type": {
        "qualType": "__clang_svint8x4_t"
      },
      "inner": [
        {
          "id": "0x13b841010",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint8x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9072f0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint16x4_t",
      "type": {
        "qualType": "__clang_svint16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841030",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907358",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint32x4_t",
      "type": {
        "qualType": "__clang_svint32x4_t"
      },
      "inner": [
        {
          "id": "0x13b841050",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9073c0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svint64x4_t",
      "type": {
        "qualType": "__clang_svint64x4_t"
      },
      "inner": [
        {
          "id": "0x13b841070",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svint64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907428",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint8x4_t",
      "type": {
        "qualType": "__clang_svuint8x4_t"
      },
      "inner": [
        {
          "id": "0x13b841090",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint8x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907490",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint16x4_t",
      "type": {
        "qualType": "__clang_svuint16x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410b0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9074f8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint32x4_t",
      "type": {
        "qualType": "__clang_svuint32x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907560",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svuint64x4_t",
      "type": {
        "qualType": "__clang_svuint64x4_t"
      },
      "inner": [
        {
          "id": "0x13b8410f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svuint64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9075c8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat16x4_t",
      "type": {
        "qualType": "__clang_svfloat16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841110",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907630",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat32x4_t",
      "type": {
        "qualType": "__clang_svfloat32x4_t"
      },
      "inner": [
        {
          "id": "0x13b841130",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat32x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907698",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svfloat64x4_t",
      "type": {
        "qualType": "__clang_svfloat64x4_t"
      },
      "inner": [
        {
          "id": "0x13b841150",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svfloat64x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907700",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svbfloat16x4_t",
      "type": {
        "qualType": "__clang_svbfloat16x4_t"
      },
      "inner": [
        {
          "id": "0x13b841170",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svbfloat16x4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907768",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVBool_t",
      "type": {
        "qualType": "__SVBool_t"
      },
      "inner": [
        {
          "id": "0x13b841190",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVBool_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9077d0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svboolx2_t",
      "type": {
        "qualType": "__clang_svboolx2_t"
      },
      "inner": [
        {
          "id": "0x13b8411b0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svboolx2_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907838",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__clang_svboolx4_t",
      "type": {
        "qualType": "__clang_svboolx4_t"
      },
      "inner": [
        {
          "id": "0x13b8411d0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__clang_svboolx4_t"
          }
        }
      ]
    },
    {
      "id": "0x13b9078a0",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__SVCount_t",
      "type": {
        "qualType": "__SVCount_t"
      },
      "inner": [
        {
          "id": "0x13b8411f0",
          "kind": "BuiltinType",
          "type": {
            "qualType": "__SVCount_t"
          }
        }
      ]
    },
    {
      "id": "0x13b907948",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__builtin_ms_va_list",
      "type": {
        "qualType": "char *"
      },
      "inner": [
        {
          "id": "0x13b907900",
          "kind": "PointerType",
          "type": {
            "qualType": "char *"
          },
          "inner": [
            {
              "id": "0x13b8404b0",
              "kind": "BuiltinType",
              "type": {
                "qualType": "char"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b9079b8",
      "kind": "TypedefDecl",
      "loc": {},
      "range": {
        "begin": {},
        "end": {}
      },
      "isImplicit": true,
      "name": "__builtin_va_list",
      "type": {
        "qualType": "char *"
      },
      "inner": [
        {
          "id": "0x13b907900",
          "kind": "PointerType",
          "type": {
            "qualType": "char *"
          },
          "inner": [
            {
              "id": "0x13b8404b0",
              "kind": "BuiltinType",
              "type": {
                "qualType": "char"
              }
            }
          ]
        }
      ]
    },
    {
      "id": "0x13b907a28",
      "kind": "ObjCCategoryDecl",
      "loc": {
        "offset": 11,
        "file": "/path/to/Example.m",
        "line": 1,
        "col": 12,
        "tokLen": 9
      },
      "range": {
        "begin": {
          "offset": 0,
          "col": 1,
          "tokLen": 1
        },
        "end": {
          "offset": 49,
          "line": 3,
          "col": 2,
          "tokLen": 3
        }
      },
      "isInvalid": true,
      "name": "SomeExtension",
      "interface": {
        "id": "0x0"
      },
      "implementation": {
        "id": "0x0"
      },
      "inner": [
        {
          "id": "0x13b907b88",
          "kind": "ObjCMethodDecl",
          "loc": {
            "offset": 37,
            "line": 2,
            "col": 1,
            "tokLen": 1
          },
          "range": {
            "begin": {
              "offset": 37,
              "col": 1,
              "tokLen": 1
            },
            "end": {
              "offset": 46,
              "col": 10,
              "tokLen": 1
            }
          },
          "name": "f"[1]    35259 segmentation fault  clang -cc1 -ast-dump=json
```
&lt;/details&gt;

And here's what it gives when running in text mode:

&lt;details&gt;
  &lt;summary&gt;Expand&lt;/summary&gt;
```
$ clang -cc1 -ast-dump 'Example.m'
/path/to/Example.m:1:12: error: cannot find interface declaration for 'SomeClass'
    1 | @<!-- -->interface SomeClass (SomeExtension)
      |            ^
TranslationUnitDecl 0x12c040408 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt;
|-TypedefDecl 0x12c0412b0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __int128_t '__int128'
| `-BuiltinType 0x12c0409d0 '__int128'
|-TypedefDecl 0x12c041320 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0x12c0409f0 'unsigned __int128'
|-TypedefDecl 0x12c105a00 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit SEL 'SEL *'
| `-PointerType 0x12c041380 'SEL *'
|   `-BuiltinType 0x12c040b70 'SEL'
|-TypedefDecl 0x12c105ae8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit id 'id'
| `-ObjCObjectPointerType 0x12c105a90 'id'
|   `-ObjCObjectType 0x12c105a60 'id'
|-TypedefDecl 0x12c105bc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit Class 'Class'
| `-ObjCObjectPointerType 0x12c105b70 'Class'
|   `-ObjCObjectType 0x12c105b40 'Class'
|-ObjCInterfaceDecl 0x12c105c20 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit Protocol
|-TypedefDecl 0x12c105f80 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0x12c105d50 'struct __NSConstantString_tag'
|   `-Record 0x12c105cd0 '__NSConstantString_tag'
|-TypedefDecl 0x12c105fe8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt8_t '__SVInt8_t'
| `-BuiltinType 0x12c040b90 '__SVInt8_t'
|-TypedefDecl 0x12c106050 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt16_t '__SVInt16_t'
| `-BuiltinType 0x12c040bb0 '__SVInt16_t'
|-TypedefDecl 0x12c1060b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt32_t '__SVInt32_t'
| `-BuiltinType 0x12c040bd0 '__SVInt32_t'
|-TypedefDecl 0x12c106120 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt64_t '__SVInt64_t'
| `-BuiltinType 0x12c040bf0 '__SVInt64_t'
|-TypedefDecl 0x12c106188 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint8_t '__SVUint8_t'
| `-BuiltinType 0x12c040c10 '__SVUint8_t'
|-TypedefDecl 0x12c1061f0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint16_t '__SVUint16_t'
| `-BuiltinType 0x12c040c30 '__SVUint16_t'
|-TypedefDecl 0x12c106258 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint32_t '__SVUint32_t'
| `-BuiltinType 0x12c040c50 '__SVUint32_t'
|-TypedefDecl 0x12c1062c0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint64_t '__SVUint64_t'
| `-BuiltinType 0x12c040c70 '__SVUint64_t'
|-TypedefDecl 0x12c106328 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat16_t '__SVFloat16_t'
| `-BuiltinType 0x12c040c90 '__SVFloat16_t'
|-TypedefDecl 0x12c106390 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat32_t '__SVFloat32_t'
| `-BuiltinType 0x12c040cb0 '__SVFloat32_t'
|-TypedefDecl 0x12c1063f8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat64_t '__SVFloat64_t'
| `-BuiltinType 0x12c040cd0 '__SVFloat64_t'
|-TypedefDecl 0x12c106460 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBFloat16_t '__SVBFloat16_t'
| `-BuiltinType 0x12c040cf0 '__SVBFloat16_t'
|-TypedefDecl 0x12c1064c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x2_t '__clang_svint8x2_t'
| `-BuiltinType 0x12c040d10 '__clang_svint8x2_t'
|-TypedefDecl 0x12c106530 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x2_t '__clang_svint16x2_t'
| `-BuiltinType 0x12c040d30 '__clang_svint16x2_t'
|-TypedefDecl 0x12c106598 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x2_t '__clang_svint32x2_t'
| `-BuiltinType 0x12c040d50 '__clang_svint32x2_t'
|-TypedefDecl 0x12c106600 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x2_t '__clang_svint64x2_t'
| `-BuiltinType 0x12c040d70 '__clang_svint64x2_t'
|-TypedefDecl 0x12c106668 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x2_t '__clang_svuint8x2_t'
| `-BuiltinType 0x12c040d90 '__clang_svuint8x2_t'
|-TypedefDecl 0x12c1066d0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x2_t '__clang_svuint16x2_t'
| `-BuiltinType 0x12c040db0 '__clang_svuint16x2_t'
|-TypedefDecl 0x12c106738 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x2_t '__clang_svuint32x2_t'
| `-BuiltinType 0x12c040dd0 '__clang_svuint32x2_t'
|-TypedefDecl 0x12c1067a0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x2_t '__clang_svuint64x2_t'
| `-BuiltinType 0x12c040df0 '__clang_svuint64x2_t'
|-TypedefDecl 0x12c106808 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x2_t '__clang_svfloat16x2_t'
| `-BuiltinType 0x12c040e10 '__clang_svfloat16x2_t'
|-TypedefDecl 0x12c106870 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x2_t '__clang_svfloat32x2_t'
| `-BuiltinType 0x12c040e30 '__clang_svfloat32x2_t'
|-TypedefDecl 0x12c1068d8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x2_t '__clang_svfloat64x2_t'
| `-BuiltinType 0x12c040e50 '__clang_svfloat64x2_t'
|-TypedefDecl 0x12c106940 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x2_t '__clang_svbfloat16x2_t'
| `-BuiltinType 0x12c040e70 '__clang_svbfloat16x2_t'
|-TypedefDecl 0x12c1069a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x3_t '__clang_svint8x3_t'
| `-BuiltinType 0x12c040e90 '__clang_svint8x3_t'
|-TypedefDecl 0x12c106e10 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x3_t '__clang_svint16x3_t'
| `-BuiltinType 0x12c040eb0 '__clang_svint16x3_t'
|-TypedefDecl 0x12c106e78 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x3_t '__clang_svint32x3_t'
| `-BuiltinType 0x12c040ed0 '__clang_svint32x3_t'
|-TypedefDecl 0x12c106ee0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x3_t '__clang_svint64x3_t'
| `-BuiltinType 0x12c040ef0 '__clang_svint64x3_t'
|-TypedefDecl 0x12c106f48 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x3_t '__clang_svuint8x3_t'
| `-BuiltinType 0x12c040f10 '__clang_svuint8x3_t'
|-TypedefDecl 0x12c106fb0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x3_t '__clang_svuint16x3_t'
| `-BuiltinType 0x12c040f30 '__clang_svuint16x3_t'
|-TypedefDecl 0x12c107018 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x3_t '__clang_svuint32x3_t'
| `-BuiltinType 0x12c040f50 '__clang_svuint32x3_t'
|-TypedefDecl 0x12c107080 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x3_t '__clang_svuint64x3_t'
| `-BuiltinType 0x12c040f70 '__clang_svuint64x3_t'
|-TypedefDecl 0x12c1070e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x3_t '__clang_svfloat16x3_t'
| `-BuiltinType 0x12c040f90 '__clang_svfloat16x3_t'
|-TypedefDecl 0x12c107150 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x3_t '__clang_svfloat32x3_t'
| `-BuiltinType 0x12c040fb0 '__clang_svfloat32x3_t'
|-TypedefDecl 0x12c1071b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x3_t '__clang_svfloat64x3_t'
| `-BuiltinType 0x12c040fd0 '__clang_svfloat64x3_t'
|-TypedefDecl 0x12c107220 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x3_t '__clang_svbfloat16x3_t'
| `-BuiltinType 0x12c040ff0 '__clang_svbfloat16x3_t'
|-TypedefDecl 0x12c107288 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x4_t '__clang_svint8x4_t'
| `-BuiltinType 0x12c041010 '__clang_svint8x4_t'
|-TypedefDecl 0x12c1072f0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x4_t '__clang_svint16x4_t'
| `-BuiltinType 0x12c041030 '__clang_svint16x4_t'
|-TypedefDecl 0x12c107358 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x4_t '__clang_svint32x4_t'
| `-BuiltinType 0x12c041050 '__clang_svint32x4_t'
|-TypedefDecl 0x12c1073c0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x4_t '__clang_svint64x4_t'
| `-BuiltinType 0x12c041070 '__clang_svint64x4_t'
|-TypedefDecl 0x12c107428 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x4_t '__clang_svuint8x4_t'
| `-BuiltinType 0x12c041090 '__clang_svuint8x4_t'
|-TypedefDecl 0x12c107490 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x4_t '__clang_svuint16x4_t'
| `-BuiltinType 0x12c0410b0 '__clang_svuint16x4_t'
|-TypedefDecl 0x12c1074f8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x4_t '__clang_svuint32x4_t'
| `-BuiltinType 0x12c0410d0 '__clang_svuint32x4_t'
|-TypedefDecl 0x12c107560 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x4_t '__clang_svuint64x4_t'
| `-BuiltinType 0x12c0410f0 '__clang_svuint64x4_t'
|-TypedefDecl 0x12c1075c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x4_t '__clang_svfloat16x4_t'
| `-BuiltinType 0x12c041110 '__clang_svfloat16x4_t'
|-TypedefDecl 0x12c107630 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x4_t '__clang_svfloat32x4_t'
| `-BuiltinType 0x12c041130 '__clang_svfloat32x4_t'
|-TypedefDecl 0x12c107698 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x4_t '__clang_svfloat64x4_t'
| `-BuiltinType 0x12c041150 '__clang_svfloat64x4_t'
|-TypedefDecl 0x12c107700 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x4_t '__clang_svbfloat16x4_t'
| `-BuiltinType 0x12c041170 '__clang_svbfloat16x4_t'
|-TypedefDecl 0x12c107768 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBool_t '__SVBool_t'
| `-BuiltinType 0x12c041190 '__SVBool_t'
|-TypedefDecl 0x12c1077d0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx2_t '__clang_svboolx2_t'
| `-BuiltinType 0x12c0411b0 '__clang_svboolx2_t'
|-TypedefDecl 0x12c107838 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx4_t '__clang_svboolx4_t'
| `-BuiltinType 0x12c0411d0 '__clang_svboolx4_t'
|-TypedefDecl 0x12c1078a0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVCount_t '__SVCount_t'
| `-BuiltinType 0x12c0411f0 '__SVCount_t'
|-TypedefDecl 0x12c107948 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_ms_va_list 'char *'
| `-PointerType 0x12c107900 'char *'
|   `-BuiltinType 0x12c0404b0 'char'
|-TypedefDecl 0x12c1079b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_va_list 'char *'
| `-PointerType 0x12c107900 'char *'
|   `-BuiltinType 0x12c0404b0 'char'
`-ObjCCategoryDecl 0x12c107a28 &lt;/path/to/Example.m:1:1, line:3:2&gt; line:1:12 invalid SomeExtension
  `-ObjCMethodDecl 0x12c107b88 &lt;line:2:1, col:10&gt; col:1 + f 'void'
1 error generated.
```
&lt;/details&gt;

I'm on a mac running macOS 15.4. 

`clang -cc1 -version`:

```
Apple LLVM version 16.0.0
   (clang-1600.0.26.6)Optimized build.
```


</details>


---

