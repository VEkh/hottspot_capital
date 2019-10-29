defmodule HottspotCapital.Basket.Generator do
  alias HottspotCapital.Repo
  alias HottspotCapital.SQLQueryParser

  def basket_query() do
    """
    (
      SELECT reference.symbol AS reference_symbol,
      basket_item.symbol AS basket_item_symbol,
      CORR(reference.close, basket_item.close) AS correlation

      FROM (
        SELECT close, date, symbol FROM stock_quotes
        WHERE symbol = $symbol
        AND EXTRACT(month from date) NOT IN (1,4,7,10)
        AND date < $date_limit
      ) AS reference

      JOIN (
        SELECT close, date, symbol FROM stock_quotes
        WHERE symbol != $symbol
        AND EXTRACT(month from date) NOT IN (1,4,7,10)
        AND date < $date_limit
      ) AS basket_item
        ON reference.date = basket_item.date

      GROUP BY reference.symbol, basket_item.symbol
      HAVING CORR(reference.close, basket_item.close) IS NOT NULL
      ORDER BY 3 DESC
      LIMIT 10
    )
    """
  end

  def generate(symbol, options \\ []) do
    %{date_limit: date_limit} = merge_options(options)

    {query, params} =
      """
      SELECT
        basket_query.reference_symbol AS symbol,
        JSON_AGG(
          JSON_BUILD_OBJECT(
            basket_query.basket_item_symbol,
            basket_query.correlation
          )
        ) AS basket
      FROM #{basket_query()} AS basket_query
      GROUP BY basket_query.reference_symbol
      """
      |> SQLQueryParser.named_to_ordered_params(
        date_limit: date_limit,
        symbol: symbol
      )

    %Postgrex.Result{rows: rows} = Repo.query!(query, params)

    case rows do
      [[^symbol, basket]] -> basket
      [] -> []
    end
  end

  def merge_options(options) do
    defaults = [
      date_limit: Date.utc_today()
    ]

    defaults
    |> Keyword.merge(options)
    |> Enum.into(%{})
  end
end
