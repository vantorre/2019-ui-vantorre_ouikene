type action =
  | Signin
  | Signup
  | Login(string)
  | Password(string);

type credential = string;

type state = {
  login: credential,
  password: credential,
};

let component = ReasonReact.reducerComponent("Login");

let postExecute = (url: string, body, callback) => {
  let promise =
    Js.Promise.(
      Fetch.fetchWithInit(
        url,
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(Js.Json.stringify(body)),
          ~headers=
            Fetch.HeadersInit.make({
              "mode": "cors",
              "Content-Type": "application/json",
              "Access-Control-Allow-Origin": "*",
            }),
          (),
        ),
      )
      |> then_(Fetch.Response.json)
      |> then_(value =>
           Json.Decode.{"success": value |> field("success", bool), "message": value |> field("message", string)}
           |> (
             response => {
               switch (response##success) {
               | false => Js.Promise.resolve(response)
               | _ =>
                 callback();
                 Js.Promise.resolve(response);
               };
             }
           )
         )
    );
  ();
};

let signup = state => {
  ReasonReact.Router.push("/score") |> ignore;
};

let make = _children => {
  ...component,
  initialState: () => {login: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Login(login) => ReasonReact.Update({login, password: state.password})
    | Password(password) => ReasonReact.Update({login: state.login, password})
    | Signin =>
      ReasonReact.SideEffects(
        _self =>
          if (state.login != "" && state.password != "") {
            postExecute(
              "https://app-d6af5d0c-6efd-46de-ada6-719b9210357b.cleverapps.io/users/login",
              Json.Encode.(object_([("login", string(state.login)), ("password", string(state.password))])),
              () =>
              ReasonReact.Router.push("score")
            );
          } else {
            ReasonReact.NoUpdate;
            Js.log("Authentication Error");
          },
      )
    | Signup =>
      ReasonReact.Router.push("login");
      ReasonReact.NoUpdate;
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string("Login Page")} </h1>
      <span> {ReasonReact.string("Login")} </span>
      <input onChange={evt => self.send(Login(ReactEvent.Form.target(evt)##value))} />
      <span> {ReasonReact.string("Password")} </span>
      <input onChange={evt => self.send(Password(ReactEvent.Form.target(evt)##value))} />
      <button onClick={_ => self.send(Signin)}> {ReasonReact.string("SignIn")} </button>
      <button onClick={_ => self.send(Signup)}> {ReasonReact.string("SignUp")} </button>
    </div>,
};
