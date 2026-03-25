# [flang] verification of lowering to FIR failed: 'hlfir.eoshift' op ARRAY and BOUNDARY operands must have the same element type

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/160665
**Status:** Open
**Labels:** crash, flang:fir-hlfir

## Body

Consider the following code:
https://godbolt.org/z/f4EKjevM5
```f90
type t
  sequence
  integer :: i
endtype

type(t) :: a(1)

contains

  subroutine s()
    type t
      sequence
      integer :: i
    end type

    type(t) :: b(1)

    a = eoshift(a, 1, b(1))

  end subroutine
end
```

Flang failed at lowering:
```console
error: loc("/app/example.f90":18:5): 'hlfir.eoshift' op ARRAY and BOUNDARY operands must have the same element type
error: verification of lowering to FIR failed
```

## Comments

### Comment 1 - eugeneepshteyn

Peter pointed to this quote from F'2023:
7.5.2.4 “Two data entities have the same type if they are declared with reference to the same derived-type definition. Data
entities also have the same type if they are declared with reference to different derived-type definitions that specify
the same type name, all have the SEQUENCE attribute or all have the BIND attribute, have no components
with PRIVATE accessibility, and have components that agree in order, name, and attributes. Otherwise, they
are of different derived types.”

So both types `t` should be treated as the same type.

---

### Comment 2 - eugeneepshteyn

Possibly this function will need to be modified to consider type's names and structure:
```
/// Verification helper for checking if two types are the same.
/// Set \p allowCharacterLenMismatch to true, if character types
/// of different known lengths should be treated as the same.
template <typename Op>
static llvm::LogicalResult areMatchingTypes(Op &op, mlir::Type type1,
                                            mlir::Type type2,
                                            bool allowCharacterLenMismatch) {
```
Or perhaps something already exists in semantics that could be helpful.

---

