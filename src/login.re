type action =
  | Signin
  | Signup;

type credential = string;

type state = {
  login: credential,
  password: credential,
};

type login = string;
type password = string;

let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: () => {login: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Signin => ReasonReact.Update({login: state.login, password: state.password})
    | Signup => ReasonReact.Update({login: state.login, password: state.password})
    | login => ReasonReact.Update({login: action.value, password: state.password})
    | password => ReasonReact.Update({login: state.login, password: action.value})
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string("Login Page")} </h1>
      <span> {ReasonReact.string("Login")} </span>
      <input onChange={evt => self.send(login(ReactEvent.Form.target(evt)##value))} />
      <span> {ReasonReact.string("Password")} </span>
      <input onChange={evt => self.send(password(ReactEvent.Form.target(evt)##value))} />
      <button onClick={_ => self.send(Signin)}> {ReasonReact.string("SignIn")} </button>
      <button onClick={_ => self.send(Signup)}> {ReasonReact.string("SignUp")} </button>
    </div>,
};
