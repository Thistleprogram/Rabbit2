#include "base.hpp"
#include "mouse.hpp"
#include "beam.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "button.hpp"
#include "trophy.hpp"

void Main() {
	sw.start();
	Window::SetTitle(U"Rabbit2");
	Window::Resize(1500, 800);
	Window::SetStyle(WindowStyle::Sizable);

	//block, wall, scoreの読み込み
	String str;
	CSVData bcsv(U"Datas/blocks.csv");
	for (int i = 1; i < bcsv.rows(); i++) {
		blocks[Parse<int>(bcsv[i][0]) + 2] << Block(Rect(Parse<int>(bcsv[i][1]), Parse<int>(bcsv[i][2]), Parse<int>(bcsv[i][3]), Parse<int>(bcsv[i][4])), Parse<int>(bcsv[i][5]), Parse<int>(bcsv[i][6]), Parse<int>(bcsv[i][7]), Parse<int>(bcsv[i][8]), Parse<int>(bcsv[i][9]));
	}
	CSVData ecsv(U"Datas/enemies.csv");
	for (int i = 1; i < ecsv.rows(); i++) {
		enemies[Parse<int>(ecsv[i][0]) + 2] << Enemy(Vec2(Parse<int>(ecsv[i][3]), Parse<int>(ecsv[i][4])), ecsv[i][1] == U"1", Parse<int>(ecsv[i][2]), Parse<int>(ecsv[i][5]), Parse<int>(ecsv[i][6]));
	}
	TextReader screader(U"Datas/scores.txt");
	while (screader.readLine(str)) {
		scores << Parse<int>(str);
	}
	screader.close();
	Texture bground[10];
	for (int i = 0; i < 4; i++) {
		bground[i + 2] = Texture(U"Datas/background" + ToString(i + 1) + U".png");
	}
	bool pre_mouse = 0;

	Button backtitle(Rect(1200, 20, 300, 40), Palette::Red, 30, U"Back To Title!", 1);
	//scene==0
	const Font title(120);
	Button play(Rect(350, 400, 800, 150), Palette::Blue, 80, U"Play!", 1);
	Button credits(Rect(350, 600, 800, 150), Palette::Blue, 80, U"Credits", 1);
	//scene==1
	const Font Stages(70);
	Button stages[6] = { Button(Rect(200, 200, 600, 80), Palette::Blue, 60, U"tutorial",1),
	Button(Rect(200, 300, 600, 80), Palette::Blue, 60, U"easy-mode",0),
	Button(Rect(200, 400, 600, 80), Palette::Blue, 60, U"hard-mode",0),
	Button(Rect(200, 500, 600, 80), Palette::Blue, 60, U"Kthanid",0),
	Button(Rect(200, 600, 600, 80), Palette::Blue, 60, U"Usa-Emon",0),
	Button(Rect(200, 700, 600, 80), Palette::Blue, 60, U"Orchestra",0) , };
	String pas;
	String passwords[5] = { U"first" ,U"second", U"Rlyah",U"moment",U"Conq" };
	Rect passback(890, 250, 400, 70);
	Button Confirm(Rect(1295, 250, 200, 70), Palette::Gray, 30, U"Confirm", 1);
	//scene<=7
	Player player(Vec2(100, 200), U"Datas/rabbit.png");
	int start;
	int finish = 0;
	bool cleared = 0;
	int overed = 0;
	const Font info(50);
	const Font infos(40);
	const Font GAMEOVER(200);
	const Font expl(20);
	Rect expb(580, 60, 600, 200);
	Color expbc = Palette::Aqua;
	expbc.a = 100;
	int time = 300000;
	int ttime = 0;
	int played = 2;
	bool overs[100];
	int alenem;//生きている敵の数
	//scene==2
	Trophy tr2(Vec2(5600, 350));
	//scene==3
	Trophy tr3(Vec2(5695, 60));
	//scene==4
	Trophy tr4(Vec2(5890, 350));
	//scene==5
	Trophy tr5(Vec2(5890, 350));
	//scene==6
	Trophy tr6(Vec2(5890, 350));
	//scene==7

	//scene==8//リザルト（クリア時間、それぞれのコースの最短クリア時間、パスワード）
	const Font fastest(40);
	const Font showpas(40, Typeface::Heavy);
	//scene==9//生産者
	const Font list(30);

	scene = 0;
	start = sw.ms();

	while (System::Update()) {
		if (scene == 0) {
			//タイトル画面
			Scene::SetBackground(Palette::White);
			title(U"Rabbit2").drawAt(Scene::Center().x, 250, Palette::Black);
			play.draw();
			credits.draw();
			if (!pre_mouse && play.onclick()) {
				scene = 1;
			}
			if (!pre_mouse && credits.onclick()) {
				scene = 9;
			}
		}
		else if (scene == 1) {
			Scene::SetBackground(Palette::White);
			Stages(U"Select Stage!").drawAt(Scene::Center().x, 100, Palette::Black);
			for (int i = 0; i < 6; i++) {
				stages[i].draw();
			}
			for (int i = 0; i < 6; i++) {
				if (!pre_mouse && stages[i].onclick()) {
					scene = i + 2;
					start = sw.ms();
					finish = -1;
					cleared = 0;
					overed = 0;
					ttime = -1;
					played = i + 2;
					alenem = 0;
					for (int i = 0; i < 100; i++) {
						overs[i] = 0;
					}
					break;
				}
			}
			Color tmp = Palette::Gray;
			tmp.a = 150;
			passback.draw(tmp);
			info(U"Password").draw(900, 180, Palette::Black);
			info(pas).draw(900, 250, Palette::Black);
			Confirm.draw();
			if ((!pre_mouse && Confirm.onclick()) || KeyEnter.pressed()) {
				for (int i = 0; i < 5; i++) {
					if (passwords[i] == pas) {
						for (int j = 0; j <= i + 1; j++) {
							stages[j].valid = 1;
						}
					}
				}
			}
			TextInput::UpdateText(pas);
			backtitle.draw();
			if (!pre_mouse && backtitle.onclick()) {
				scene = 0;
			}
		}
		else if (scene <= 7) {
			alenem = 0;
			bground[scene].draw(0, 0);

			for (auto& wall : blocks[scene]) {
				wall.draw();
			}
			player.draw(Scene::DeltaTime());

			//blocks,walls
			for (auto& enemy : enemies[scene]) {
				if (enemy.hp > 0) {
					enemy.draw(Scene::DeltaTime());
					alenem++;
				}
			}
			while (!beams.empty() && beams.front().dele()) {
				beams.pop_front();
			}
			for (auto& beam : beams) {
				if (!beam.alive) continue;
				beam.draw(Scene::DeltaTime());
				if (beam.from == 0) {
					for (auto& wall : blocks[scene]) {
						if (wall.hp <= 0) continue;
						if (wall.block.intersects(beam.beam)) {
							beam.alive = 0;
							wall.hit();
						}
					}
					for (auto& enemy : enemies[scene]) {
						if (enemy.hp <= 0) continue;
						if (enemy.enemy.intersects(beam.beam)) {
							enemy.hit(beam);
						}
					}
				}
				else {
					if (player.shield && player.Shield.intersects(beam.beam)) {
						beam.alive = 0;
					}
					else if (player.prec.intersects(beam.beam)) {
						player.hit(beam);
					}
					for (auto& wall : blocks[scene]) {
						if (wall.hp <= 0) continue;
						if (wall.block.intersects(beam.beam)) {
							beam.alive = 0;
						}
					}
				}
			}
			while (!slashes.empty() && !slashes.front().alive) {
				slashes.pop_front();
			}
			for (auto& slash : slashes) {
				slash.draw(Scene::DeltaTime());
				if (slash.from == 0) {
					for (auto& wall : blocks[scene]) {
						if (wall.hp <= 0) continue;
						if (wall.block.intersects(slash.slash)) {
							wall.hit(10 * Scene::DeltaTime());
						}
					}
					for (auto& enemy : enemies[scene]) {
						if (enemy.hp <= 0) continue;
						if (enemy.enemy.intersects(slash.slash)) {
							enemy.hp -= 300 * Scene::DeltaTime();
						}
					}
				}
				else {
					if (player.prec.intersects(slash.slash) && !player.shield) {
						player.hp -= 300 * Scene::DeltaTime();
					}
				}
			}
			for (auto beam : bbuffer) {
				beams << beam;
			}
			bbuffer.clear();
			for (auto& beam : dummy) {

				beam.draw(Scene::DeltaTime());
			}
			bool tmp = cleared;
			if (scene == 2) {
				tr2.draw(player);
				cleared = tr2.cleared;
				if (ppos.x <= 800) {
					expb.draw(expbc);
					expl(U"ADキーで横移動。\nステージ内にあるトロフィーをゲットしましょう！\n制限時間が0になるとゲームオーバーです。").draw(600, 80);
				}
				if (900 <= ppos.x && ppos.x <= 1700) {
					expb.draw(expbc);
					expl(U"Wキーでジャンプをします。").draw(600, 80);
				}
				if (2050 <= ppos.x && ppos.x <= 2300) {
					expb.draw(expbc);
					expl(U"壁に触れている状態で、\n壁と反対の方向に移動しようとすると壁キックをします。").draw(600, 80);
				}
				if (2400 <= ppos.x && ppos.x <= 3000) {
					expb.draw(expbc);
					expl(U"左クリックでカーソルの方向にワイヤーを打ち出します。\n長さは緑円の範囲内です。\nワイヤーが当たったらそこに向かって引っ張られていきます。\n天井や床にもワイヤーを打つことができます。\n壁キックなどと組み合わせて壁を上りましょう。").draw(600, 80);
				}
				if (3700 <= ppos.x && ppos.x <= 4300) {
					expb.draw(expbc);
					expl(U"赤と黄色の市松模様のブロックは危険です。\nHPが減ります。").draw(600, 80);
				}
				if (4500 <= ppos.x && ppos.x <= 4750) {
					expb.draw(expbc);
					expl(U"マウススクロールで斬撃を放つことができます。\nブロックは攻撃で壊すことができます。\n右クリックでビームを打つことができます。\n攻撃力は斬撃より小さいです。\nSキーでシールドを張ることができます。\nシールドのチャージ時間は使うごとに長くなります。").draw(600, 80);
				}
				if (4700 <= ppos.x) {
					expb.draw(expbc);
					expl(U"敵はビーム（右クリック）や\n斬撃（マウススクロール）で倒すことができます。\n青い敵はビームを跳ね返します。\nまた、クリアのトロフィーは条件を満たさないと\n触れられないときがあります。").draw(600, 80);
					if (enemies[2].size() < 2) {
						enemies[2] << Enemy(Vec2(5300, 400), 0, 1, 5, 0);
						enemies[2] << Enemy(Vec2(5300, 650), 1, 0, 5, 0);
					}
				}
				if (enemies[2].size() >= 2 && enemies[2][0].hp <= 0 && enemies[2][1].hp <= 0) {
					blocks[2][18].hp = 0; blocks[2][19].hp = 0; blocks[2][20].hp = 0;
				}
			}
			else if (scene == 3) {
				tr3.draw(player);
				cleared = tr3.cleared;
			}
			else if (scene == 4) {
				tr4.draw(player);
				cleared = tr4.cleared;
				if (ppos.x <= 4000 && enemies[4][22].hp <= 0 && enemies[4][23].hp <= 0 && enemies[4][24].hp <= 0 && enemies[4][25].hp <= 0) {
					blocks[4][16].hp = 0;
				}
				if (!overs[0] && player.pos.x >= 2900) {
					enemies[scene] << Enemy(Vec2(3400, 200), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(3400, 600), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(300, 300), 0, 2, 5, 150);
					overs[0] = 1;
				}
				if (!overs[1] && player.pos.x >= 3400) {
					enemies[scene] << Enemy(Vec2(3900, 200), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(3900, 600), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(3700, 600), 0, 2, 5, 150);
					overs[1] = 1;
				}
				if (!overs[2] && player.pos.x >= 4100) {
					enemies[scene] << Enemy(Vec2(4400, 200), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(4400, 600), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(4700, 600), 0, 2, 5, 150);
					overs[2] = 1;
				}
				if (!overs[3] && player.pos.x >= 4400) {
					enemies[scene] << Enemy(Vec2(4800, 200), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(4800, 600), 0, 2, 5, 150);
					enemies[scene] << Enemy(Vec2(4800, 600), 0, 2, 5, 150);
					overs[3] = 1;
				}
				if (ppos.x <= 350) {
					expb.draw(expbc);
					expl(U"時間をかけすぎるとよくないことが起きるかも…？").draw(600, 80);
				}
				if (1750 <= ppos.x && ppos.x <= 2750) {
					expb.draw(expbc);
					expl(U"敵がたくさんいるように見えますが、ほとんど分身で\n実体は5体しかいません。\n青い敵はビームを反射してきます。").draw(600, 80);
				}
				if (ppos.x >= 4940 && !overs[4]) {
					enemies[scene].clear();
					for (int i = 0; i < (sw.ms() - start) / 500; i++) {
						enemies[scene] << Enemy(RandomVec2(RectF(4940, 20, 1010, 430)), RandomBool(0.4), 2, 5, 100);
					}
					blocks[scene] << Block(Rect(4900, 320, 20, 100), 1000000, 0, 0, 0, 0);
					overs[4] = 1;
				}
				if (ppos.x >= 4940 && alenem == 0) {
					blocks[scene][46].hp = 0; blocks[scene][47].hp = 0; blocks[scene][48].hp = 0;
				}
			}
			else if (scene == 5) {
				tr5.draw(player);
				cleared = tr5.cleared;
				
			}
			else if (scene == 6) {
				tr6.draw(player);
				cleared = tr6.cleared;
				if (!overs[0]) {
					for (int i = 0; i < 1000; i++) {
						enemies[scene] << Enemy(RandomVec2(RectF(20, 20, 500, 660)), RandomBool(0.4), 2, 5, 100);
					}
					overs[0] = 1;
				}
				enemies[scene] << Enemy(RandomVec2(RectF(20, 20, 1200, 660)), RandomBool(0.4), 2, 5, 100);
				if (alenem == 0) {
					blocks[scene][4].hp = 0; blocks[scene][5].hp = 0; blocks[scene][6].hp = 0;
				}
			}
			else if (scene == 7) {

			}
			player.hp = std::max(player.hp, 0);
			Rect(0, 700, 1200, 100).draw(Palette::Gray);
			Rect(1200, 0, 300, 800).draw(Palette::Aqua);
			infos(U"HP").draw(1220, 130, Palette::Black);
			infos(ToString(player.hp)).draw(1300, 130, Palette::Black);
			Rect(1220, 200, 260, 50).draw(Palette::White);
			Rect(1220, 200, player.hp * 260 / 1000, 50).draw(Palette::Green);
			infos(U"Rest Time").draw(1220, 300, Palette::Black);
			infos(U"{}'{}''{}"_fmt((int)(time - (sw.ms() - start)) / 60000, (int)((time - (sw.ms() - start)) / 1000) % 60, (int)((time - (sw.ms() - start)) % 1000))).draw(1280, 370, Palette::Black);
			Rect(1220, 430, 260, 50).draw(Palette::White);
			Rect(1220, 430, (time - (sw.ms() - start)) * 260 / time, 50).draw(Palette::Green);
			if (player.shield) {
				infos(U"Shield Time").draw(1220, 500, Palette::Black);
				infos(U"{}''{}"_fmt(player.altime / 1000, (player.altime % 1000))).draw(1280, 570, Palette::Black);
				Rect(1220, 630, 260, 50).draw(Palette::White);
				Rect(1220, 630, player.altime * 260 / 3000, 50).draw(Palette::Green);
			}
			else {
				infos(U"Shield Recast").draw(1220, 500, Palette::Black);
				infos(U"{}''{}"_fmt(std::max(0, (int)(player.eval - (sw.ms() - player.unfolded)) / 1000), std::max(0, (int)(player.eval - (sw.ms() - player.unfolded))) % 1000)).draw(1280, 570, Palette::Black);
				Rect(1220, 630, 260, 50).draw(Palette::Green);
				if (player.eval > 0) {
					Rect(1480 - std::max(0, (int)(player.eval - (sw.ms() - player.unfolded))) * 260 / player.eval, 630, std::max(0, (int)(player.eval - (sw.ms() - player.unfolded))) * 260 / player.eval, 50).draw(Palette::White);
				}
				if (player.eval < sw.ms() - player.unfolded) {
					Rect(1220, 630, 260, 50).draw(Palette::Lime);
				}
			}
			Rect(100, 720, 1000, 50).draw(Palette::Palegreen);
			if (ppos.x <= 600) {
				Rect(100, 720, 200, 50).draw(Palette::Thistle);
				Rect(300, 720, 800, 50).draw(Palette::White);
			}
			else if (ppos.x <= ssize - 600) {
				Rect((ppos.x - 600) * 1000 / ssize + 100, 720, 200, 50).draw(Palette::Thistle);
				Rect((ppos.x + 600) * 1000 / ssize + 100, 720, 1000 - (ppos.x + 600) * 1000 / ssize, 50).draw(Palette::White);
			}
			else {
				Rect(900, 720, 200, 50).draw(Palette::Thistle);
			}
			if (!tmp && cleared) {
				ttime = (int)(sw.ms() - start);
				scores[scene - 2] = std::min(scores[scene - 2], ttime);
				TextWriter textwriter(U"Datas/scores.txt");
				for (int i = 0; i < 6; i++) {
					textwriter << scores[i];
				}
				textwriter.close();
			}
			if (finish < 0 && cleared) {
				finish = sw.ms();
				overed = 1;
			}
			else if (finish < 0 && (player.hp <= 0 || sw.ms() - start >= time)) {
				finish = sw.ms();
				overed = 2;
			}
			if (overed == 1) {
				GAMEOVER(U"CLEAR!!!").drawAt(Scene::Center(), Palette::Gold);
			}
			if (overed == 2) {
				GAMEOVER(U"GAME OVER!").drawAt(Scene::Center(), Palette::Red);
			}
			if (finish > 0 && sw.ms() - finish >= 3000) {
				scene = 8;
			}
		}
		else if (scene == 8) {//結果
			Stages(U"Result").drawAt(Scene::Center().x, 100, Palette::Black);
			if (overed == 1) {
				title(U"CLEARED").drawAt(Scene::Center().x, 190, Palette::Gold);
				list(U"Password in stage select").draw(1100, 100, Palette::Black);
				showpas(U"'" + passwords[played - 2] + U"'").drawAt(1270, 180, Palette::Violet);
			}
			else {
				title(U"YOU FAILED").drawAt(Scene::Center().x, 190, Palette::Red);
			}
			if (ttime < 0) {
				Stages(U"Took Time  :  infinity").drawAt(Scene::Center().x + 20, 300, Palette::Black);
			}
			else {
				Stages(U"Took Time  :  {}'{}''{}"_fmt(ttime / 60000, (ttime / 1000) % 60, ttime % 1000)).drawAt(Scene::Center().x + 20, 300, Palette::Black);
			}
			Stages(U"Fastests").draw(300, 380, Palette::Black);
			for (int i = 0; i < 6; i++) {
				fastest(U"{}"_fmt(stages[i].str)).draw(330, 480 + i * 50, Palette::Black);
				if (scores[i] == 100000000) {
					fastest(U"non cleared").draw(550, 480 + i * 50, Palette::Black);
				}
				else {
					fastest(U"{}'{}''{}"_fmt(scores[i] / 60000, (scores[i] / 1000) % 60, scores[i] % 1000)).draw(550, 480 + i * 50, Palette::Black);
				}
			}
		}
		else {//クレジット
			Scene::SetBackground(Palette::Navy);
			Stages(U"作った人").draw(100, 50, Palette::White);
			list(U"原作").draw(100, 200, Palette::White);
			list(U"分かったらThistleまで").draw(700, 200, Palette::White);
			list(U"原案\n実装\nデバッグ\nデザイン\nテスト\nプロデュース").draw(100, 300, Palette::White);
			list(U"Thistle").draw(700, 500, Palette::White);
			Line(50, 270, 1200, 270).draw(LineStyle::RoundDot, 6, Palette::Orange);
			backtitle.draw();
			if (!pre_mouse && backtitle.onclick()) {
				scene = 0;
			}
		}
		pre_mouse = MouseL.pressed();
	}
}