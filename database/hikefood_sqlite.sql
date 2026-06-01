-- SQLite версия базы данных HikeFoodList
-- Конвертировано из MySQL dump

PRAGMA foreign_keys = OFF;

-- Таблица: Bakery
DROP TABLE IF EXISTS "Bakery";
CREATE TABLE "Bakery" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  "id_product2" INTEGER DEFAULT NULL,
  "id_product3" INTEGER DEFAULT NULL,
  "id_product4" INTEGER DEFAULT NULL,
  "id_product5" INTEGER DEFAULT NULL,
  "id_product6" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product2") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product3") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product4") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product5") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product6") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Bakery_id_product1" ON "Bakery" ("id_product1");
CREATE INDEX "idx_Bakery_id_product2" ON "Bakery" ("id_product2");
CREATE INDEX "idx_Bakery_id_product3" ON "Bakery" ("id_product3");
CREATE INDEX "idx_Bakery_id_product4" ON "Bakery" ("id_product4");
CREATE INDEX "idx_Bakery_id_product5" ON "Bakery" ("id_product5");
CREATE INDEX "idx_Bakery_id_product6" ON "Bakery" ("id_product6");

-- Таблица: Bread
DROP TABLE IF EXISTS "Bread";
CREATE TABLE "Bread" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Bread_id_product1" ON "Bread" ("id_product1");

-- Таблица: Breakfast
DROP TABLE IF EXISTS "Breakfast";
CREATE TABLE "Breakfast" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  "id_product2" INTEGER DEFAULT NULL,
  "id_product3" INTEGER DEFAULT NULL,
  "id_product4" INTEGER DEFAULT NULL,
  "id_product5" INTEGER DEFAULT NULL,
  "id_product6" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product2") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product3") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product4") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product5") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product6") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Breakfast_id_product1" ON "Breakfast" ("id_product1");
CREATE INDEX "idx_Breakfast_id_product2" ON "Breakfast" ("id_product2");
CREATE INDEX "idx_Breakfast_id_product3" ON "Breakfast" ("id_product3");
CREATE INDEX "idx_Breakfast_id_product4" ON "Breakfast" ("id_product4");
CREATE INDEX "idx_Breakfast_id_product5" ON "Breakfast" ("id_product5");
CREATE INDEX "idx_Breakfast_id_product6" ON "Breakfast" ("id_product6");

-- Таблица: CalorieTable (основная таблица, должна быть создана первой)
DROP TABLE IF EXISTS "CalorieTable";
CREATE TABLE "CalorieTable" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "calorie" INTEGER DEFAULT NULL,
  "avg_gram_for_person" INTEGER DEFAULT NULL,
  "avg_kcal_for_person" INTEGER DEFAULT NULL,
  "russ_name" TEXT DEFAULT NULL
);

-- Таблица: Dinner
DROP TABLE IF EXISTS "Dinner";
CREATE TABLE "Dinner" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  "id_product2" INTEGER DEFAULT NULL,
  "id_product3" INTEGER DEFAULT NULL,
  "id_product4" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product2") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product3") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product4") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Dinner_id_product1" ON "Dinner" ("id_product1");
CREATE INDEX "idx_Dinner_id_product2" ON "Dinner" ("id_product2");
CREATE INDEX "idx_Dinner_id_product3" ON "Dinner" ("id_product3");
CREATE INDEX "idx_Dinner_id_product4" ON "Dinner" ("id_product4");

-- Таблица: Lunch
DROP TABLE IF EXISTS "Lunch";
CREATE TABLE "Lunch" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  "id_product2" INTEGER DEFAULT NULL,
  "id_product3" INTEGER DEFAULT NULL,
  "id_product4" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product2") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product3") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY ("id_product4") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Lunch_id_product1" ON "Lunch" ("id_product1");
CREATE INDEX "idx_Lunch_id_product2" ON "Lunch" ("id_product2");
CREATE INDEX "idx_Lunch_id_product3" ON "Lunch" ("id_product3");
CREATE INDEX "idx_Lunch_id_product4" ON "Lunch" ("id_product4");

-- Таблица: Snack
DROP TABLE IF EXISTS "Snack";
CREATE TABLE "Snack" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Snack_id_product1" ON "Snack" ("id_product1");

-- Таблица: Sweet
DROP TABLE IF EXISTS "Sweet";
CREATE TABLE "Sweet" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_Sweet_id_product1" ON "Sweet" ("id_product1");

-- Таблица: WithBread
DROP TABLE IF EXISTS "WithBread";
CREATE TABLE "WithBread" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" TEXT DEFAULT NULL,
  "id_product1" INTEGER DEFAULT NULL,
  FOREIGN KEY ("id_product1") REFERENCES "CalorieTable"("id") ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX "idx_WithBread_id_product1" ON "WithBread" ("id_product1");

PRAGMA foreign_keys = ON;

-- Вставка данных в CalorieTable (сначала, так как есть внешние ключи)
INSERT INTO "CalorieTable" VALUES (1,'grechka',330,80,264,'гречка');
INSERT INTO "CalorieTable" VALUES (2,'gerkules',345,60,207,'геркулес');
INSERT INTO "CalorieTable" VALUES (3,'psheno',300,60,180,'пшено');
INSERT INTO "CalorieTable" VALUES (4,'rice',330,70,231,'рис');
INSERT INTO "CalorieTable" VALUES (5,'pasta',333,120,266,'макароны');
INSERT INTO "CalorieTable" VALUES (6,'manka',320,40,128,'манка');
INSERT INTO "CalorieTable" VALUES (7,'potato mash',370,50,185,'картоф.пюре');
INSERT INTO "CalorieTable" VALUES (8,'chechevitsa',295,40,118,'чечевица');
INSERT INTO "CalorieTable" VALUES (9,'muka',319,45,144,'мука');
INSERT INTO "CalorieTable" VALUES (10,'cheese',400,30,120,'сыр');
INSERT INTO "CalorieTable" VALUES (11,'sausage',550,30,165,'колбаса');
INSERT INTO "CalorieTable" VALUES (12,'beef stew',220,50,110,'тушен.говядина');
INSERT INTO "CalorieTable" VALUES (13,'pork stew',349,50,175,'тушен.свинина');
INSERT INTO "CalorieTable" VALUES (14,'dried pork',565,30,170,'сублимир.свинина');
INSERT INTO "CalorieTable" VALUES (15,'dried potato',319,50,160,'сушен.картофель');
INSERT INTO "CalorieTable" VALUES (16,'dried vegetables',320,15,48,'сушен.овощи');
INSERT INTO "CalorieTable" VALUES (17,'dried mango',340,30,102,'сушен.манго');
INSERT INTO "CalorieTable" VALUES (18,'raisin',260,30,78,'изюм');
INSERT INTO "CalorieTable" VALUES (19,'dried apricots',215,30,65,'курага');
INSERT INTO "CalorieTable" VALUES (20,'walnut',654,30,196,'грец.орех');
INSERT INTO "CalorieTable" VALUES (21,'peanut',550,30,165,'арахис');
INSERT INTO "CalorieTable" VALUES (22,'cashew',536,30,161,'кешью');
INSERT INTO "CalorieTable" VALUES (23,'condensed milk',335,30,101,'сгущ.молоко');
INSERT INTO "CalorieTable" VALUES (24,'dry milk',494,30,148,'сухое молоко');
INSERT INTO "CalorieTable" VALUES (25,'cookies',430,30,129,'печенье');
INSERT INTO "CalorieTable" VALUES (26,'dry bananas',245,30,74,'вяленые бананы');
INSERT INTO "CalorieTable" VALUES (27,'halva',470,30,141,'халва');
INSERT INTO "CalorieTable" VALUES (28,'chokolate',500,30,150,'шоколад');
INSERT INTO "CalorieTable" VALUES (29,'sugar',390,30,117,'сахар');
INSERT INTO "CalorieTable" VALUES (30,'sherbet',380,30,114,'щербет');
INSERT INTO "CalorieTable" VALUES (31,'waffles',300,30,90,'вафли');
INSERT INTO "CalorieTable" VALUES (32,'sushki',370,30,111,'сушки');
INSERT INTO "CalorieTable" VALUES (33,'maggi sauce',0,9,0,'соус Магги');
INSERT INTO "CalorieTable" VALUES (34,'fast grechka',345,80,276,'хлопья гречневые');
INSERT INTO "CalorieTable" VALUES (35,'bulgur',340,70,238,'булгур');
INSERT INTO "CalorieTable" VALUES (36,'cereal bars',350,30,105,'злаков. батончики');
INSERT INTO "CalorieTable" VALUES (37,'egg powder',396,10,40,'яичный порошок');
INSERT INTO "CalorieTable" VALUES (38,'soda',0,1,0,'сода');
INSERT INTO "CalorieTable" VALUES (39,'peas mash',300,50,150,'горох.пюре');
INSERT INTO "CalorieTable" VALUES (40,'bran(otrubi)',120,20,24,'отруби');
INSERT INTO "CalorieTable" VALUES (41,'dry chicken',520,30,156,'сублимир.курица');
INSERT INTO "CalorieTable" VALUES (42,'fast dry soup',323,35,113,'готовая смесь суповая');
INSERT INTO "CalorieTable" VALUES (43,'breadcrumbs',300,25,75,'сухари');
INSERT INTO "CalorieTable" VALUES (44,'galets',365,25,91,'галеты');
INSERT INTO "CalorieTable" VALUES (45,'crispBread',340,25,85,'хлебцы');
INSERT INTO "CalorieTable" VALUES (46,'fast corn',340,60,204,'кукурузн. хлопья (каша)');
INSERT INTO "CalorieTable" VALUES (47,'chicken pate',300,30,90,'курин.паштет');
INSERT INTO "CalorieTable" VALUES (48,'fish pate',260,30,78,'рыбн.паштет');
INSERT INTO "CalorieTable" VALUES (49,'turkey pate',185,30,56,'паштет из индейки');
INSERT INTO "CalorieTable" VALUES (50,'fish preserves',120,50,60,'рыбные консервы');
INSERT INTO "CalorieTable" VALUES (51,'dateBar',397,30,119,'финиковый батончик');

-- Вставка данных в другие таблицы
INSERT INTO "Bakery" VALUES (1,'Pancakes (оладьи)',9,24,37,29,38,23);
INSERT INTO "Bread" VALUES (1,'breadCrumbs (сухари)',43);
INSERT INTO "Bread" VALUES (2,'galets (галеты)',44);
INSERT INTO "Bread" VALUES (3,'crispBread (хлебцы)',45);
INSERT INTO "Breakfast" VALUES (1,'fast grechka porridge (греч.хлопья каша)',34,40,NULL,NULL,NULL,NULL);
INSERT INTO "Breakfast" VALUES (2,'milk grechka porridge (мол.каша гречнев.)',1,29,24,40,NULL,NULL);
INSERT INTO "Breakfast" VALUES (3,'milk gerkules porridge (мол.геркулес.каша)',2,29,24,18,40,NULL);
INSERT INTO "Breakfast" VALUES (4,'milk fast corn porridge (мол.кукурузн.хлопья каша)',46,29,24,18,40,NULL);
INSERT INTO "Breakfast" VALUES (5,'milk rice porridge (мол.рисовая каша)',4,29,24,17,40,NULL);
INSERT INTO "Dinner" VALUES (1,'Ragout&DryChicken (Рагу с сублимир.куриц)',15,41,16,NULL);
INSERT INTO "Dinner" VALUES (2,'Ragout&DriedPork (Рагу с сублимир.свинин.)',15,14,16,NULL);
INSERT INTO "Dinner" VALUES (3,'ChechevSoup&DryChiken (Суп с чечевиц.и субл.кур.)',8,15,16,41);
INSERT INTO "Dinner" VALUES (4,'SoupPasta&DryChicken (суп с вермиш. и субл.кур.)',42,41,16,NULL);
INSERT INTO "Dinner" VALUES (5,'VegetSoup&Dryork (суп с овощами и субл.свин.)',14,15,16,NULL);
INSERT INTO "Dinner" VALUES (6,'Rice&FishPreserves (рис с рыбн.консервами)',4,50,16,NULL);
INSERT INTO "Lunch" VALUES (1,'Grechka&Beef Stew (гречка и тушен.говядина)',1,12,33,16);
INSERT INTO "Lunch" VALUES (2,'PeasMash&Pork Stew (горох.пюре и тушен.свинина)',39,13,33,NULL);
INSERT INTO "Lunch" VALUES (3,'Grechka&Dried pork (гречка и сублимир.свинина)',1,14,33,16);
INSERT INTO "Lunch" VALUES (4,'PeasMash&DriedPork (горох.пюре и сублимир.свинина)',39,14,33,NULL);
INSERT INTO "Lunch" VALUES (5,'PotatoMash&BeefStew (картоф.пюре и тушен.говядина)',7,12,33,NULL);
INSERT INTO "Lunch" VALUES (6,'PotatoMash&DriedPork (карт.пюре и сублимир.свин.)',7,14,33,NULL);
INSERT INTO "Lunch" VALUES (7,'Rice&PorkStew (рис и тушен.свинина)',4,13,33,16);
INSERT INTO "Lunch" VALUES (8,'Bulgur&BeefStew (булгур и тушен.говядина)',35,12,33,16);
INSERT INTO "Lunch" VALUES (9,'Pasta&DriedChicken (макароны и сублимир.курица)',5,41,33,NULL);
INSERT INTO "Lunch" VALUES (10,'Pasta&PorkStew (макароны и тушен.свинина)',5,13,33,16);
INSERT INTO "Lunch" VALUES (11,'Pasta&BeefStew (макароны и тушен.говядина)',5,12,33,16);
INSERT INTO "Snack" VALUES (1,'dried mango (суш.манго)',17);
INSERT INTO "Snack" VALUES (2,'walnut (грец.орех)',20);
INSERT INTO "Snack" VALUES (3,'peanut, (арахис)',21);
INSERT INTO "Snack" VALUES (4,'cashew (кешью)',22);
INSERT INTO "Snack" VALUES (5,'dry bananas (вяленые бананы)',26);
INSERT INTO "Snack" VALUES (6,'cereal bars (злак.батон)',36);
INSERT INTO "Snack" VALUES (7,'dateBar (фиников. батончик)',51);
INSERT INTO "Sweet" VALUES (1,'sushki (сушки)',32);
INSERT INTO "Sweet" VALUES (2,'waffles (вафли)',31);
INSERT INTO "Sweet" VALUES (3,'sherbet (щербет)',30);
INSERT INTO "Sweet" VALUES (4,'chokolate (шоколад)',28);
INSERT INTO "Sweet" VALUES (5,'halva (халва)',27);
INSERT INTO "Sweet" VALUES (6,'cookies (печенье)',25);
INSERT INTO "WithBread" VALUES (1,'chickenPate (курин.паштет)',47);
INSERT INTO "WithBread" VALUES (2,'fishPate (рыбн.паштет)',48);
INSERT INTO "WithBread" VALUES (3,'turkeyPate (инд.паштет)',49);