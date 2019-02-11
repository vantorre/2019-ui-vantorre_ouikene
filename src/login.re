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

let make = _children => {
  ...component,
  initialState: () => {login: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Signin => ReasonReact.Update({login: state.login, password: state.password})
    | Signup => ReasonReact.Update({login: state.login, password: state.password})
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
