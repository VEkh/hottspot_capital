defmodule HottspotCapital.Basket.Generator do
  alias HottspotCapital.Repo
  alias HottspotCapital.SQLQueryParser

  def basket_query() do
    """
    (
      SELECT reference.symbol AS reference_symbol,
      basket_item.symbol AS basket_item_symbol,
      CORR(reference.close, basket_item.close) AS correlation

      FROM stock_quotes AS reference
      JOIN stock_quotes AS basket_item
        ON reference.date = basket_item.date
        AND reference.symbol != basket_item.symbol
      WHERE reference.symbol = $symbol
      GROUP BY reference.symbol, basket_item.symbol
      ORDER BY 3 DESC
      LIMIT 10
    )
    """
  end

  def generate(symbol) do
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
      |> SQLQueryParser.named_to_ordered_params(symbol: symbol)

    %Postgrex.Result{rows: [[^symbol, basket]]} = Repo.query!(query, params)

    basket
  end
end