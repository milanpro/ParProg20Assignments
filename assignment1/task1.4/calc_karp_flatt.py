import pandas as pd


# Because the pandas pivot function doesn't work
def multiIndex_pivot(df, index=None, columns=None, values=None):
    # https://github.com/pandas-dev/pandas/issues/23955
    output_df = df.copy(deep=True)
    if index is None:
        names = list(output_df.index.names)
        output_df = output_df.reset_index()
    else:
        names = index
    output_df = output_df.assign(tuples_index=[tuple(i) for i in output_df[names].values])
    if isinstance(columns, list):
        output_df = output_df.assign(tuples_columns=[tuple(i) for i in output_df[columns].values])  # hashable
        output_df = output_df.pivot(index='tuples_index', columns='tuples_columns', values=values)
        output_df.columns = pd.MultiIndex.from_tuples(output_df.columns, names=columns)  # reduced
    else:
        output_df = output_df.pivot(index='tuples_index', columns=columns, values=values)
    output_df.index = pd.MultiIndex.from_tuples(output_df.index, names=names)
    return output_df


benchmarks = pd.read_csv('measurement.csv')
benchmarks = benchmarks.groupby(['strategy', 'nthreads', 'npoints']).mean().reset_index()

benchmarks = benchmarks\
    .pipe(multiIndex_pivot, index=['strategy', 'npoints'], columns=['nthreads'], values='runtime')\
    .reset_index()

columns = [2, 3, 4, 6, 8, 16, 32, 64]

for col in columns:
    benchmarks[col, ] = benchmarks[1, ] / benchmarks[col, ]
    benchmarks[col, ] = (1 / benchmarks[col, ] - 1 / col) / (1 - 1 / col)
    benchmarks.rename(columns={col: f'q{col}'}, inplace=True)

benchmarks.drop(columns=1, inplace=True)
benchmarks.to_csv('metrics.csv', index=False)
