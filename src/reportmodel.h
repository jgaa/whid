#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class ReportModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    enum class Mode {
      DAYS,
      WEEKS,
      MONTHS,
      YEARS
    };

    explicit ReportModel();


    void setFilter(const QString& filter);

signals:



    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent) const override;

private:
    void fetchCache(int row);
    int computeBaseline(int row);

    struct RowCache {
        int row = -1;
        QString label;
        int work = 0; // seconds
        int baseline = 0;

        int diff() const noexcept { return work - baseline; }
    } row_cache_;

    Mode mode_ = Mode::MONTHS;
};

#endif // REPORTMODEL_H
