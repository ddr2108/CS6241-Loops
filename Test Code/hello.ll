; ModuleID = 'hello.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1
@str = private unnamed_addr constant [12 x i8] c"hello world\00"
@str1 = private unnamed_addr constant [12 x i8] c"hello world\00"

; Function Attrs: nounwind uwtable
define void @third() #0 {
entry:
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([12 x i8]* @str, i64 0, i64 0))
  ret void
}

; Function Attrs: nounwind uwtable
define void @second() #0 {
entry:
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([12 x i8]* @str1, i64 0, i64 0))
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  tail call void @second()
  br label %while.cond

while.cond:                                       ; preds = %while.cond, %entry
  %call = tail call i32 @rand() #2
  %rem = srem i32 %call, 20
  %cmp = icmp eq i32 %rem, 3
  br i1 %cmp, label %while.end, label %while.cond

while.end:                                        ; preds = %while.cond
  tail call void @third()
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @rand() #1

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 199752)"}
