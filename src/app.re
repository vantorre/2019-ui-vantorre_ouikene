type page =
  | Login
  | Score;

module App = {
  type state = {route: page};
  type action =
    | UpdatePage(page);
  let component = ReasonReact.reducerComponent("App");
  let make = _children => {
    ...component,
    initialState: () => {route: Login},
    didMount: self => {
      let watcherID =
        ReasonReact.Router.watchUrl(url =>
          switch (url.path) {
          | ["score"] => self.send(UpdatePage(Score))
          | ["/login"] => self.send(UpdatePage(Login))
          | _ => self.send(UpdatePage(Login))
          }
        );
      self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    },
    reducer: (action, _state) =>
      switch (action) {
      | UpdatePage(route) => ReasonReact.Update({route: route})
      },

    render: ({state}) =>
      <div>
        {switch (state.route) {
         | Score => <Score />
         | Login => <Login />
         }}
      </div>,
  };
};

ReactDOMRe.renderToElementWithId(<App />, "main");
