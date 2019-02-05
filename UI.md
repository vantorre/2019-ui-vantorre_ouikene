# Use tennis API

During this training we will design a visual couter for a tennis game.

## UI Components

### Introduction

The Document Object Model (DOM) is a programming interface for HTML, XML and SVG documents. The DOM provides a representation of the document as a structured group of nodes and objects that have properties and methods. Nodes can also have event handlers attached to them, and once that event is triggered the event handlers get executed. Essentially, it connects web pages to scripts or programming languages.

The main idea of react is to compose a web application with components.

The DOM has 2 main problems :

- lack of encapsulation
  - Web components & Angular solution is Shadow DOM
  - React solution is react component
- low performance to alter three nodes
  - React use Virtual DOM : The Virtual DOM is an abstraction of the HTML DOM. It is lightweight and detached from the browser-specific implementation details. Since the DOM itself was already an abstraction, the virtual DOM is, in fact, an abstraction of an abstraction.
  - There are other implementations of virtual DOM (Mithril, Inferno, Vue.js, ...)

### React

[React](https://reactjs.org/) is a declarative, efficient, and flexible JavaScript library for building user interfaces.
React has a few different kinds of components.

This is an exemple of "Stateless components" which is just a function returning JSX :

```js
const myComponent = () => (
  <div className="members-list">
    <h1> A list of members </h1>
    <ul>
      <li>Negan</li>
      <li>Carl</li>
    </ul>
  </div>
);
```

This funny tag syntax is neither a string nor HTML.

It is called [JSX](https://reactjs.org/docs/introducing-jsx.html), and it is a syntax extension to JavaScript. JSX may remind you of a template language, but it comes with the full power of JavaScript.

JSX contains regular HTML tags or other React components.
There are few differences, but you can notice that _class_ property is replace by _className_

React is a UI library, we will consider to generate React component from ReasonML, but remember there are lot of way to use it... But it's not the purpose of this training.

### Reason React

[ReasonReact](https://reasonml.github.io/reason-react/) is a safer, simpler way to build React components, in Reason.

This is a sample of an Hello World component.

```OCaml
module App = {
  let component = ReasonReact.statelessComponent("App");
  /* underscores before names indicate unused variables. We name them for clarity */
  let make = (~name, _children) => {
    ...component, /* spread the template's other defaults into here  */
    render: _self => <div> (ReasonReact.string("hello" ++ name ++ "!")) </div>
  };
};

/* Mount the <App> component the DOM element with the id "main" */
ReactDOMRe.renderToElementWithId(<App name="MIAGE" />, "main");
```

A component in Reason is a **ReasonML module** with a `let` binding which must be named `component` and a a function `make` that return a record with a render function.
This component is a Stateless component which means it cannot have state. We will see below how to build a Statefull component

### Build a first ReasonReact Application

1. Install ReasonReact bindings

```sh
npm i -S react react-dom reason-react
```

2. Modify `bsconfig.json` the config file for [Buckelscript](https://bucklescript.github.io/docs/en/build-configuration)

```json
{
  ...
  "reason": { "react-jsx": 2 },
  "bs-dependencies": ["reason-react"],
  "bs-dev-dependencies": ["@glennsl/bs-jest"],
  ...
}
```

3. Create a root HTML file [index.html](./index.html)

```html
<html>
  <body>
    <div id="main"></div>
    <script src="./src/app.re"></script>
  </body>
</html>
```

4. Create a bundle

Install [parcel bundler](https://parceljs.org/reasonML.html) _Alternativly you can also use Webpack_

```sh
npm install parcel-bundler --save-dev
```

Then you can build you bundle

```sh
npx parcel index.html
```

Parcel build a bundle in [dist folder](./dist) but also come with a server to test on your local (don't use parcel server to serve apps in production). Go to [http://localhost:1234](http://localhost:1234) to test you app.

#### Statefull components

[ReasonReact stateful components](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer) implement the Elm Architecture (like Redux). If that word doesn't mean anything to you, just think of it as a state machine.

The logic of every ReasonReact Compoent will break up into three cleanly separated parts:

- state — the state of your module (one component)
- reducer — a way to update your state
- render — a way to view your state as JSX

It results a **one way data-flow** :

```
        ------------
    --->|  reducer |-------update----
    |   ------------                |
    |                               ˇ
    |                           ---------
  send action                   | state |
    |                           ---------
    |   ------------                |
    ----| render   |<------call------
        ------------
```

#### Score component

We will create a [reducer component](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer.html)

Edit [./src/score.re](./src/score.re) :

```OCaml
open Tennis;

type action =
  | ScorePlayerOne
  | ScorePlayerTwo
  | NewGame;

type state = {score};

let component = ReasonReact.reducerComponent("Score");

let make = _children => {
  ...component,
  initialState: () => {score: newGame},
  reducer: (action, state) =>
    switch action {
    | ScorePlayerOne =>
      ReasonReact.Update({score: score(state.score, PlayerOne)})
    | ScorePlayerTwo =>
      ReasonReact.Update({score: score(state.score, PlayerTwo)})
    | NewGame => ReasonReact.Update({score: newGame})
    },
  render: self =>
    <div>
      <h1> (ReasonReact.string("Tennis Game Counter")) </h1>
      <h2> (string_of_score(self.state.score) |> ReasonReact.string) </h2>
      (
        switch self.state.score {
        | Game(_) =>
          <button onClick=((_) => self.send(NewGame))>
            (ReasonReact.string("New Game"))
          </button>
        | _ =>
          <div>
            (ReasonReact.string("Who scored ?"))
            <button onClick=((_) => self.send(ScorePlayerOne))>
              (ReasonReact.string(" Player 1 "))
            </button>
            <button onClick=((_) => self.send(ScorePlayerTwo))>
              (ReasonReact.string(" Player 2 "))
            </button>
          </div>
        }
      )
    </div>
};
```

Then we will mount it our App in [./src/app.re](./src/app.re)

```OCaml
module App = {
  let component = ReasonReact.statelessComponent("App");
  let make = _children => {...component, render: _self => <Score />};
};

ReactDOMRe.renderToElementWithId(<App />, "main");
```

## Exercice

- Create a register and a login components
- Create routes to implement the following behavior /
  - When I arrive to the app, I see the register page.
    - On this page I can register or navigate to the login form
    - When I register, I am automatically logged in with the registered account.
  - When I am on the login page I can login or navigate to the register form.
  - When I logged in I am redirect to the score page.
- You may manage the upper level logic in the App component
- You must use the login backend you create for the 2nd trainning to register/login.
- When it works on local, deploy backend (login service) and frontend (this application) on clever-cloud
  - you need to create a second nodejs app
  - I provide you source code for [a simple static files server](./server/index.js) that will serve your bundled application

I give you some [npm scripts](./package.json#6) to facilitate development flow but feel free to add yours.

To achieve this training you will need to understand some new concepts.
At this point you should be able to understand the docs.
Some pointers (not exhaustive) :

- [ReasonReact Router](https://reasonml.github.io/reason-react/docs/en/router) for the routing
- [bs-fetch](https://github.com/reasonml-community/bs-fetch) to fetch the API
- [Js.Promise](https://reasonml.github.io/docs/en/promise) to manage promise chains
