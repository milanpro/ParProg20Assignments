-module(nqueens).
-import(lists,[sum/1]).
-export([main/1]).
-export([nqueens/1]).
-export([queens_asnyc/5]).

main([N]) -> 
    nqueens(list_to_integer(N)).

log_n(X, N) ->
    math:log(X) / math:log(N).

nqueens(1) -> io:format("1~n");
nqueens(N) -> 
    Cpus = erlang:system_info(schedulers_online),
    Spawndepth = ceil(log_n(Cpus, N)),
    Pid = spawn_link(nqueens, queens_asnyc, [N, N, [], self(), Spawndepth]),
    Solutions = receive {Pid, Count} -> Count end,
    io:format("~w~n", [Solutions]).

same_row({_X, _Y}, []) ->
    false;
same_row({_X, Y}, [{_X2, Y} | _Tail]) ->
    true;
same_row({X, Y}, [{_X2, _Y2} | Tail]) ->
    same_row({X, Y}, Tail).

same_diagonal({_X, _Y}, []) ->
    false;
same_diagonal({X, Y}, [{X2, Y2} | Tail]) ->
    XDiff = abs(X - X2),
    YDiff = abs(Y - Y2),
    case XDiff == YDiff of
        true -> true;
        false -> same_diagonal({X, Y}, Tail)
    end.

is_legal(New_queen, Queens) ->
    Row_ok = not same_row(New_queen, Queens),
    Diag_ok = not same_diagonal(New_queen, Queens),
    Row_ok and Diag_ok.

queens_asnyc(0, _N, _Queens, Owner, _Spawndepth) ->
    Owner ! {self(), 1};
queens_asnyc(Column, N, Queens, Owner, 0) ->
    Rows = lists:seq(1, N),
    Solution_list = [ queens_snyc(Column - 1, N, [ {Column, Row} | Queens]) 
                    || Row <- Rows, is_legal({Column, Row}, Queens) ],
    Solutions = sum(Solution_list),
    Owner ! {self(), Solutions};
queens_asnyc(Column, N, Queens, Owner, Spawndepth) ->
    Rows = lists:seq(1, N),
    Pids = [ spawn_link(nqueens, queens_asnyc, [Column - 1, N, [ {Column, Row} | Queens], self(), Spawndepth - 1]) 
            || Row <- Rows, is_legal({Column, Row}, Queens)],
    Solution_list = [ receive {Pid, Count} -> Count end || Pid <- Pids ],
    Solutions = sum(Solution_list),
    Owner ! {self(), Solutions}.

queens_snyc(0, _N, _Queens) ->
    1;
queens_snyc(Column, N, Queens) ->
    Rows = lists:seq(1, N),
    Solution_list = [ queens_snyc(Column - 1, N, [ {Column, Row} | Queens]) 
                    || Row <- Rows, is_legal({Column, Row}, Queens) ],
    Solutions = sum(Solution_list),
    Solutions.
