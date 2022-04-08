open Binaryen;

let start = () => {
  let mod_ = Module.create();
  Module.set_features(mod_, [Module.Feature.all]);

  ignore @@
  Function.add_function(
    mod_,
    "is_odd",
    Type.create([|Type.int32|]),
    Type.int32,
    [||],
    Expression.If.make(
      mod_,
      Expression.Binary.make(
        mod_,
        Op.le_u_int32,
        Expression.Local_get.make(mod_, 0, Type.int32),
        Expression.Const.make(mod_, Literal.int32(1l)),
      ),
      Expression.Binary.make(
        mod_,
        Op.eq_int32,
        Expression.Local_get.make(mod_, 0, Type.int32),
        Expression.Const.make(mod_, Literal.int32(1l)),
      ),
      Expression.Call.make_return(
        mod_,
        "is_even",
        [
          Expression.Binary.make(
            mod_,
            Op.sub_int32,
            Expression.Local_get.make(mod_, 0, Type.int32),
            Expression.Const.make(mod_, Literal.int32(1l)),
          ),
        ],
        Type.int32,
      ),
    ),
  );

  ignore @@
  Function.add_function(
    mod_,
    "is_even",
    Type.create([|Type.int32|]),
    Type.int32,
    [||],
    Expression.If.make(
      mod_,
      Expression.Binary.make(
        mod_,
        Op.le_u_int32,
        Expression.Local_get.make(mod_, 0, Type.int32),
        Expression.Const.make(mod_, Literal.int32(1l)),
      ),
      Expression.Binary.make(
        mod_,
        Op.eq_int32,
        Expression.Local_get.make(mod_, 0, Type.int32),
        Expression.Const.make(mod_, Literal.int32(0l)),
      ),
      Expression.Call.make_return(
        mod_,
        "is_odd",
        [
          Expression.Binary.make(
            mod_,
            Op.sub_int32,
            Expression.Local_get.make(mod_, 0, Type.int32),
            Expression.Const.make(mod_, Literal.int32(1l)),
          ),
        ],
        Type.int32,
      ),
    ),
  );

  ignore @@
  Function.add_function(
    mod_,
    "wrapper",
    Type.create([||]),
    Type.int32,
    [||],
    Expression.Call.make(
      mod_,
      "is_odd",
      [Expression.Const.make(mod_, Literal.int32(99999l))],
      Type.int32,
    ),
  );

  ignore @@ Export.add_function_export(mod_, "wrapper", "wrapper");

  prerr_endline("Unoptimized Binaryen IR:");
  Module.print(mod_);
  Module.optimize(mod_);
  prerr_endline("\nOptimized Binaryen IR:");
  Module.print(mod_);
};
