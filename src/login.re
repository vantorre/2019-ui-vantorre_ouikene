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

let postExecute = (url: string, body) =>
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
    |> then_(value => {
         Js.log(value);
         Js.Promise.resolve(value);
       })
  );

let signin = state => {
  postExecute("https://app-d6af5d0c-6efd-46de-ada6-719b9210357b.cleverapps.io/users/login", state);
  ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"));
};

let make = _children => {
  ...component,
  initialState: () => {login: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Signin => signin(state)
    | Signup => ReasonReact.SideEffects(_ => ReasonReact.Router.push(Signup))
    | Login(login) => ReasonReact.Update({login, password: state.password})
    | Password(password) => ReasonReact.Update({login: state.login, password})
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
