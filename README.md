[version-shield]: https://img.shields.io/static/v1?label=Версия&message=2024.12.0&color=green
[version]: https://github.com/dentra/esphome-tion/releases/
[license-shield]: https://img.shields.io/static/v1?label=Лицензия&message=MIT&color=orange&logo=license
[license]: https://opensource.org/licenses/MIT
[esphome-release-shield]: https://img.shields.io/static/v1?label=ESPHome&message=2024.10.3&color=green&logo=esphome
[esphome-release]: https://github.com/esphome/esphome/releases/
[open-in-vscode-shield]: https://img.shields.io/static/v1?label=+&message=Открыть+в+VSCode&color=blue&logo=visualstudiocode
[open-in-vscode]: https://open.vscode.dev/dentra/esphome-tion
[telegram-shield]: https://img.shields.io/static/v1?label=Поддержка&message=Телеграм&logo=telegram&color=blue
[telegram]: https://t.me/esphome_tion
[donate-tinkoff-shield]: https://img.shields.io/static/v1?label=Пожертвования&message=Т-Банк&color=yellow
[donate-tinkoff]: https://www.tinkoff.ru/cf/3dZPaLYDBAI
[donate-boosty-shield]: https://img.shields.io/static/v1?label=Пожертвования&message=Boosty&color=red
[donate-boosty]: https://boosty.to/dentra
[blog-shield]: https://img.shields.io/static/v1?label=Новости&message=Телеграм&logo=telegram&color=blue
[blog]: https://t.me/dentra_blog

[![Version][version-shield]][version]
[![ESPHome release][esphome-release-shield]][esphome-release]
[![License][license-shield]][license]
[![Telegram][telegram-shield]][telegram]
[![Blog][blog-shield]][blog]
[![Support author][donate-tinkoff-shield]][donate-tinkoff]
[![Support author][donate-boosty-shield]][donate-boosty]
[![Open in Visual Studio Code][open-in-vscode-shield]][open-in-vscode]

# Tion

English version of this page is available via [google translate](https://github-com.translate.goog/dentra/esphome-tion?_x_tr_sl=ru&_x_tr_tl=en&_x_tr_hl=en&_x_tr_pto=wapp).

Компонент ESPHome для управления бризерами `Tion` с помощью ESP в вашей системе управления
умным домом. Поддерживаются Home Assistant [API](https://esphome.io/components/api.html) и
любые другие системы управления умным домом через протокол
[MQTT](https://esphome.io/components/mqtt.html).

Поддерживаемые модели и протоколы:

- Tion 4S BLE
- Tion 3S BLE
- Tion Lite BLE
- Tion 4S UART
- Tion 3S UART
- Tion O2 Mac UART (бризер должен иметь возможность подключения RF-модуля, наличие самого модуля не обязательно)
- Tion Lite UART (будет доступно в следующей версии)

Интеграция позволяет управлять следующими функциями:

- Включение/Выключение
- Обогрев
- Целевая температуры нагрева
- Скорость притока воздуха
- Звуковое оповещение (для O2 только если физически включено на бризере)
- Световое оповещение (только для 4S и Lite)
- Режим приток/рециркуляция (только для 4S и 3S)
- Режим приток/рециркуляция/смешанный (только для 3S)
- Гибко настраиваемые пресеты
- Режим "Турбо"
- Настройка времени пресета "Турбо"
- Сброс ресурса фильтров (кроме O2)
- Подтверждение сброса ресурса фильтра (кроме O2)

Дополнительно осуществляется мониторинг следующих показателей:

- Температура снаружи
- Температура внутри
- Текущая потребляемая мощность нагревателя (только для 4S и Lite)
- Оставшееся время жизни фильтра (кроме O2)
- Индикация о требующейся замене/очистке фильтра (кроме O2)
- Оставшееся время работы режима "Турбо"
- Счетчик прошедшего воздуха (только для 4S и Lite)
- Текущая производительностью бризера
- Версия программного обеспечения бризера
- Измерение энергопотребления (только для 4S и Lite)
- Контроль состояния заслонки (только для Lite)
- Внутренняя температура (только для 4S и Lite)

А так же:

- Конфигурация пресетов сервисом
- Задание максимальной температуры нагрева 30°C, в том числе для модели Lite
- Дамп и сброс внутренних таймеров (только для 4S)
- Защита от обмерзания (только для 3S и O2, в 4S и Lite это неотключаемая встроенная возможность)
- Контроль и зависимость состояния нагревателя от состояния заслонки (только для 3S и 4S)
- Поддержка управления с помощью штатного пульта бризеров изначально не поддерживающих эту
  возможность, например, O2 Mac или 3S без BLE модуля.

Полный список возможностей смотрите в [инструкции по конфигурации](CONFIGURATION.md).

> [!CAUTION]
>
> ## ⚠️ **ВНИМАНИЕ: Все что вы делаете, вы делаете исключительно на свой страх и риск!**

## Подключение

Доступно два вида подключения BLE и UART.

BLE подключение работает так же как ваш пульт или официальное приложение Tion Remote.

UART подключение различно для разных моделей бризеров.

### UART-подключение Tion 4S

Для UART-подключения `Tion 4S` используется штатный интеграционный разъем бризера.
Рекомендуется приобрести стик [Lilygo T-Dongle S3](https://github.com/Xinyuan-LilyGO/T-Dongle-S3),
проще всего это сделать на AliExpress. Или собрать самостоятельно на базе ESP32.

> [!IMPORTANT]
> Для работы режима UART, прошивка бризера обязательно должна быть не ниже `02D2`,
> ниже работать не будет (основная на которой сейчас все проверяю `02E0`).
>
> Версия прошивки `03CD` имеет баг и так же не совместима, исправление производителем доступно только с версии `03E8`.
>
> Для апгрейда или даунгрейда встроенной прошивки бризера можно использовать проект
> [tion-web](https://dentra.github.io/tion-web/), где можно провести эти манипуляции прямо в браузере.

> [!IMPORTANT]
> Сборка и работа компонента на ESP8266 возможна, но стабильность не
> гарантируется, так же, в этом направлении, не будет оказана никакая поддержка,
> используйте только ESP32!

> [!IMPORTANT]
> При работе в UART-режиме, бризер требует опроса не реже одного раза в 8-10 сек,
> в противном случае тион отключает питание на интеграционном порту. OTA-обновление,
> в зависимости от удаленности от роутера и зашумленности эфира, может занимать более
> продолжительное время, в связи с этим иногда обновление может не завершится или стик
> может не успеть перегрузится. В зависимости от того что произошло может потребоваться
> выключить и повторно включить бризер. В случае неуспешного обновления будет работать
> предыдущая прошивка. В редких случаях может потребоваться повторная прошивка через USB.

### UART-подключение Tion 3S

Для UART-подключения `Tion 3S` используется штатный, но доступный только при небольшой
разборке бризера, разъем. Здесь рекомендуется использовать любую ESP8266
(ESP32 не тянет по питанию) используя [схему подключения](https://github.com/dentra/tion-hardware/blob/master/3s/NodeMCUv3-Tion.pdf).
При использовании ESP-01S не будет доступно подключение штатного модуля BLE,
в остальных случаях подключение будет полным.

> Буду благодарен за любые эксперименты по использованию современных модулей ESP32,
> например, серий C6, S3, C3 или S2.

### UART-подключение Tion O2 Mac

Для UART-подключения `Tion O2 Mac` используется штатный, но доступный только при небольшой
разборке бризера, разъем. Требуются начальные навыки работы с паяльником для подключения
разъема типа XH2.54 4pin male к ESP (распиновку см. [здесь](https://github.com/dentra/tion-hardware/blob/master/o2/pin_schema.jpg))

Уточняйте доступность по приобретению готового переходника для подключения стика
[Lilygo T-Dongle S3](https://github.com/Xinyuan-LilyGO/T-Dongle-S3).

> [!IMPORTANT]
> Сборка и работа компонента на ESP8266 возможна, но стабильность не
> гарантируется, так же, в этом направлении, не будет оказана никакая поддержка,
> рекомендуется использовать только ESP32!

По вопросам подключения велкам в [чат Telegram][telegram].

## Прошивка

Вы можете загрузить и использовать примеры конфигурации для
[Tion 4S BLE](configs/tion-4s-ble.yaml), [Tion 4S UART](configs/tion-4s-uart.yaml),
[Tion Lite BLE](configs/tion-lt-ble.yaml), [Tion 3S BLE](configs/tion-3s-ble.yaml),
[Tion 3S UART](configs/tion-3s-uart.yaml) и [Tion O2 UART](configs/tion-o2-uart.yaml)
все файлы с подробным описанием (на английском).
В примерах базовой сущностью выступает компонент Climate,
вы можете изменить его на Fan или даже обычный Switch и Number в качестве
управляющих элементов, смотрите [инструкцию по конфигурации](CONFIGURATION.md).

- Скачайте конфигурацию соответствующую модели вашего бризера
- Измените секцию `substitutions` согласно вашим предпочтениям
- Измените набор подключаемых пакетов по вашему вкусу
- Добавьте или удалите необходимы сущности согласно [инструкции по конфигурации](CONFIGURATION.md)
- Поместите модифицированный файл в директорию с конфигурацией ESPHome
- Запустите сборку и прошивку вашей конфигурации
- Добавьте появившееся устройство в Home Assistant

По прошивке доступна [видео-инструкция](https://t.me/esphome_tion/13868).

> [!TIP]
> MAC-адрес для работы в режиме BLE можно посмотреть в приложении Tion Remote,
> в приложении MagicAir может отображаться другой адрес.

> [!IMPORTANT]
> Первая прошивка обязательно должна быть по проводу, прошивка пустышкой esphome не считается таковой!

> [!IMPORTANT]
> Для модели 4S, прошивка бризера обязательно должна быть от 02D2 до 0356 включительно, посмотреть ее можно в приложении Tion Remote.
> С версиями `02D0` и `03CD` точно работать не будет!

## Использование в режиме BLE

После [прошивки](#прошивка) и перед первым использование вам необходимо ввести
свой бризер в режим сопряжения (см. инструкцию) и только потом включать ESP или
провести перезагрузку ESP с помощью функции `Restart`.

Дополнительно, только для `Tion 3S`, необходимо нажать кнопку `Pair` в Home Assistant.

> [!NOTE]
> Рекомендуется использовать ESP c двумя ядрами, например из линейки ESP32 или ESP32-S3.

> [!IMPORTANT]
> Для стабильной работы модели Lite, прошивка бризера должна быть не ниже 0570

## Использование в режиме UART

Никаких дополнительный действий не требуется.

## OTA-обновление

В режиме BLE - нет никаких ограничений.

В режиме UART для всех бризеров кроме Tion 4S - нет никаких ограничений.

В режиме UART только для Tion 4S - читайте секцию Important в разделе [UART-подключение Tion 4S](#uart-подключение-tion-4s).

## Полезности

### Просмотр и/или сброс внутренних таймеров Tion 4S

Добавьте в свою конфигурацию пакет `tion_4s_timers.yaml`:

```yaml
packages:
  tion:
---
files:
---
- packages/tion_4s_timers.yaml
```

## Использование с системами УД отличными от Home Assistant

Вы можете использовать этот компонент с любой системой УД через протокол MQTT.
Для этого в файле конфигурации найдите и раскомментируйте секцию `mqtt`, дополнительные параметры
broker, port и т.д. установите согласно настройкам вашей системы УД.
Со списком всех параметров можно ознакомиться на странице [документации](https://esphome.io/components/mqtt.html).

### Использование с УД Sprut.Hub

Необходимо настроить MQTT согласно описанию из секции выше.

Начиная с версии Sprut.Hub [beta] 1.10.2 (13054) и при включении пакета spruthub.yaml, все подхватиться автоматически.

Для версии ниже необходимо установить [шаблон](https://github.com/l0rda/sprut-tion) от [@l0rda](https://github.com/l0rda).

Дополнительную помощь всегда можно получить в [группе Telegram][telegram].

## Принцип работы и тонкая настройка

### Получение состояния

Состояние бризера храниться исключительно в самом бризере и синхронизируется
раз в настраиваемый интервал времени `tion.update_interval` или сразу же
после отработки запроса на изменение. Если состояние получить невозможно, то об этом
информирует бинарный сенсор с типом `state`, таймаут получения ответа на запрос
состояния настраивается `tion.state_timeout` и должен быть меньше
интервала опроса `tion.update_interval`.

### Изменение состояния

Изменение состояния происходит относительно последнего ответа на запрос состояния
или предыдущего ответа запроса на изменение. Так как ответ может прийти медленней
чем придет новый запрос (актуально для скриптов) реализован механизм пакетной
отправки команд на изменение. Поступивший запрос на изменение стартует таймер,
время сработки которого задается параметром `tion.batch_timeout`, каждый последующий
запрос до сработки таймера объединяется с предыдущим и рестартует таймер, после
сработки таймера объединенный запрос отправляется бризеру на выполнение.

### Отправка команд

Все запросы выстраиваются в очередь и выполняются с интервалом `vport.command_interval`.
Размер очереди задается параметром `vport.command_size`. Минимальный интервал `0s` будет
срабатывать один раз в итерацию основного цикла.

## Планы на будущее

- ~~Поддержка UART-подключения `Tion 4S`~~
- ~~Поддержка UART-подключения `Tion 3S`~~
- ~~Поддержка UART-подключения `Tion O2 Mac`~~
- Поддержка UART-подключения `Tion Lite`
- ~~Управление сбросом ресурса фильтров~~
- Автоматическое управление притоком от внешнего датчика CO2 (работает в экспериментальном режиме)
- ~~Автоматическое обновление~~ (доступно при покупке готового устройства)
- ~~Управление через штатный пульт~~
- Возможность работы бризера в режиме KIV

## Решение проблем и поддержка новых функций

Не стесняйтесь открывать [задачи](https://github.com/dentra/esphome-tion/issues) для сообщений об ошибках и запросов новых функций.

Так же вы можете воспользоваться [группой в Telegram][telegram].

## Ваша благодарность

Если этот проект оказался для вас полезен и/или вы хотите поддержать его
дальнейшее развитие, то всегда можно оставить вашу благодарность
[переводом на карту][donate-tinkoff] или
[подпиской][donate-boosty] и обычной звездой проекту.

## Коммерческое использование

По вопросам коммерческого использования или заказной разработки/доработки,
обращайтесь в [личные сообщения телеграм](https://t.me/dtrachuk)
